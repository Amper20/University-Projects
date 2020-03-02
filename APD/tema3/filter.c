#include "mpi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int cl(float num) {

    int inum = (int)num;
    
    if (num == inum)
        return inum;
    
    return inum + 1;

}

unsigned char ***image, ***tmp;
int width;
int height;
int grayscale; 
int P, rank;

//aplica filtru dat ca parametru pe imaginea image rez intermediar e salvat in tmp
void filter(float kernel[3][3]){
    
    int factor = cl((double)height/P);

    int start = rank*factor - 20;
    if(start < 0) start = 0;

    int stop = start + factor + 40;
    if(stop > height) stop = height;
        
    for(int i = start; i < stop; i++)
        for(int j = 0; j < width; j++){
            for(int k = 0; k < (grayscale ? 1 : 3); k++){
                float prod = 0;
                for(int x = -1; x < 2; x++){
                    for(int y = -1; y < 2; y++){
                        int pozx = y + j;
                        int pozy = x + i;
                        if( pozy >= 0 && pozy < height && pozx < width && pozx >= 0)
                            prod += kernel[x+1][y+1]*((float)image[pozy][pozx][k]);
                    }
                }
                if(prod <= 0.0) prod = 0.0;
                if(prod >= 255.0) prod = 255.0;
                tmp[i][j][k] = (unsigned char)prod;
            }
        }
    
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            for(int k = 0; k < (grayscale ? 1 : 3); k++)
                image[i][j][k] = tmp[i][j][k];

}

int main (int argc, char *argv[]){

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 

    //citeste imaginea data ca input
    FILE *file_pointer = fopen(argv[1],"r");
    char buff[1000];
    fgets(buff, 1000, file_pointer);
    if(buff[1] == '5') grayscale = 1;
    else grayscale = 0;
    fgets(buff, 1000, file_pointer);
    if(buff[0] == '#') fgets(buff, 1000, file_pointer);
    sscanf(buff, "%d %d", &width, &height);
    fgets(buff, 1000, file_pointer);
    
    image = (unsigned char ***)malloc(sizeof(unsigned char **)*height);
    tmp = (unsigned char ***)malloc(sizeof(unsigned char **)*height);
    
    for(int i = 0; i < height; i++){
        
        image[i] = (unsigned char **)malloc(sizeof(unsigned char *)*width);
        tmp[i] = (unsigned char **)malloc(sizeof(unsigned char *)*width);
        
        for(int j = 0 ; j < width; j++){
            
            image[i][j] = (unsigned char *)malloc(sizeof(unsigned char)*(grayscale ? 1 : 3));
            tmp[i][j] = (unsigned char *)malloc(sizeof(unsigned char)*(grayscale ? 1 : 3));
            
            if(grayscale)
                fscanf(file_pointer,"%c", &image[i][j][0]);
            else{
                fscanf(file_pointer,"%c", &image[i][j][0]);
                fscanf(file_pointer,"%c", &image[i][j][1]);
                fscanf(file_pointer,"%c", &image[i][j][2]);
            }
        }

    }

    fclose(file_pointer);
    //aplica filtrele
    for(int i = 3; i < argc; i++){
        
        if(!strcmp("smooth",argv[i])){
            float kernel[3][3] = { {(double)1.0/9, (double)1.0/9, (double)1.0/9}, {(double)1.0/9, (double)1.0/9, (double)1.0/9}, {(double)1.0/9, (double)1.0/9, (double)1.0/9} };
            filter(kernel);
        }

        if(!strcmp("blur",argv[i])){
            float kernel[3][3] ={ {(double)1.0/16, (double)2.0/16, (double)1.0/16}, {(double)2.0/16, (double)4.0/16, (double)2.0/16}, {(double)1.0/16, (double)2.0/16, (double)1.0/16} };
            filter(kernel);
        }

        if(!strcmp("sharpen",argv[i])){
            float kernel[3][3] = { {(double)0.0, (double)-2.0/3, (double)0.0}, {(double)-2.0/3, (double)11.0/3, (double)-2.0/3}, {(double)0.0, (double)-2.0/3, (double)0.0} };
            filter(kernel);
        }

        if(!strcmp("mean",argv[i])){
            float kernel[3][3] = { {(double)-1.0, (double)-1.0, (double)-1.0}, {(double)-1.0, (double)9.0, (double)-1.0}, {(double)-1.0, (double)-1.0, (double)-1.0} };
            filter(kernel);
        }

        if(!strcmp("emboss",argv[i])){
            float kernel[3][3] = { {(double)0.0, (double)-1.0, (double)0.0}, {(double)0.0, (double)0.0, (double)0.0}, {(double)0.0, (double)1.0, (double)0.0} };
            filter(kernel);
        }

    }
    //compune imaginea finala
    if(rank == 0){

        for(int i = 1; i < P; i++){
            
            int factor = cl((double)height/P); 
            
            int start = i*factor - 4;
            if(start < 0) start = 0;

            int stop = start + factor + 8;
            if(stop > height) stop = height;

            unsigned char *buff = (unsigned char *)malloc(sizeof(unsigned char)*width*(grayscale ? 1 : 3));

            if(grayscale)
                for(int j = start; j < stop; j++){
                    MPI_Recv(buff, width, MPI_UNSIGNED_CHAR, i, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    for(int k = 0; k < width; k++){
                        image[j][k][0] = buff[k];
                    }
                }
            else
                for(int j = start; j < stop; j++){
                    MPI_Recv(buff, width*3, MPI_UNSIGNED_CHAR, i, j, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    for(int k = 0,index = 0; k < width; k++, index++){
                        image[j][k][0] = buff[index]; index++;
                        image[j][k][1] = buff[index]; index++;
                        image[j][k][2] = buff[index];
                    }
                } 
            free (buff);

        }
  
        //write image
        file_pointer = fopen(argv[2],"w");
        
        if(grayscale) fprintf(file_pointer, "%s", "P5\n");
        else fprintf(file_pointer, "%s", "P6\n");

        fprintf(file_pointer, "%d %d\n", width, height);
        fprintf(file_pointer, "%d\n", 255);
        
        for(int i = 0; i < height; i++)
            for(int j = 0 ; j < width; j++){
                if(grayscale)
                    fprintf(file_pointer,"%c", image[i][j][0]);
                else{
                    fprintf(file_pointer,"%c", image[i][j][0]);
                    fprintf(file_pointer,"%c", image[i][j][1]);
                    fprintf(file_pointer,"%c", image[i][j][2]);
                }
            }

        fclose(file_pointer);
    
    }else{
        
        int factor = cl((double)height/P); 
        
        int start = rank*factor - 4;
        if(start < 0) start = 0;

        int stop = start + factor + 8;
        if(stop > height) stop = height;
        
        if(grayscale)
            for(int i = start; i < stop; i++){
                unsigned char *buff = (unsigned char *)malloc(sizeof(unsigned char)*width*(grayscale ? 1 : 3));
                for(int k = 0; k < width; k++)
                    buff[k] = image[i][k][0];
                MPI_Send(buff, width, MPI_UNSIGNED_CHAR, 0, i, MPI_COMM_WORLD);                
            }
        else
            for(int i = start; i < stop; i++){
                unsigned char *buff = (unsigned char *)malloc(sizeof(unsigned char)*width*(grayscale ? 1 : 3));
                for(int k = 0,index = 0; k < width; k++,index++){
                    buff[index] = image[i][k][0]; index++;
                    buff[index] = image[i][k][1]; index++;
                    buff[index] = image[i][k][2];
                }
                MPI_Send(buff, width*3, MPI_UNSIGNED_CHAR, 0, i, MPI_COMM_WORLD);
            }
    }
    
    MPI_Finalize();
}