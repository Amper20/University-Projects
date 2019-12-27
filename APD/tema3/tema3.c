#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//a pixel has 3 channels r g b
typedef struct{
    unsigned char val[3];
} pixel;

//struct that is used to store an image
typedef struct{
    int width, height;
    int type; //1 grayscale, 3 rgb
    int maxVal;
    pixel **pixels;
} image;

#define MAX_LEN 1000
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

char str[MAX_LEN];
int numtasks = 0, id = 0, start, stop;
float smooth[3][3] = { {(float)1/9, (float)1/9, (float)1/9}, {(float)1/9, (float)1/9, (float)1/9}, {(float)1/9, (float)1/9, (float)1/9} };
float zero[3][3] = { {(float)0, (float)0, (float)0}, {(float)0, (float)1, (float)0}, {(float)0, (float)0, (float)0} };
float blur[3][3] = { {(float)1/16, (float)2/16, (float)1/16}, {(float)2/16, (float)4/16, (float)2/16}, {(float)1/16, (float)2/16, (float)1/16} };
float sharpen[3][3] = { {(float)0, (float)-2/3, (float)0}, {(float)-2/3, (float)11/3, (float)-2/3}, {(float)0, (float)-2/3, (float)0} };
float mean[3][3] = { {(float)-1, (float)-1, (float)-1}, {(float)-1, (float)9, (float)-1}, {(float)-1, (float)-1, (float)-1} };
float emboss[3][3] = { {(float)0, (float)-1, (float)0}, {(float)0, (float)0, (float)0}, {(float)0, (float)1, (float)0} };
int padding = 0;

//function reads and returns an image
//if type is P5 grayscale values are stored in r channel
image* read_image( char *fileName){

    image *ret = (image *)malloc(sizeof(image));
    FILE *fptr = fopen(fileName,"r");

    fgets(str, MAX_LEN, fptr);   
    ret->type = 3; 
    if(!strcmp(str, "P5\n"))
        ret->type = 1;

    fgets(str, MAX_LEN, fptr);
    if(str[0] == '#')
        fgets(str, MAX_LEN, fptr);
    sscanf(str, "%d %d", &ret->width, &ret->height);
    fgets(str, MAX_LEN, fptr);
    sscanf(str, "%d", &ret->maxVal);
    ret->pixels = (pixel **)malloc(sizeof(pixel *)*ret->height);

    for(int i = 0; i < ret->height; i++){
        ret->pixels[i] = (pixel *)malloc(sizeof(pixel)*ret->width);
        for(int j = 0 ; j < ret->width; j++){
            for(int k = 0 ; k < ret->type; k++)
                fscanf(fptr,"%c",&ret->pixels[i][j].val[k]);
        }
    }
    fclose(fptr);
    return ret;
}

//function writes an image structure to specified file
void write_image( char *fileName, image* img){
    FILE *fptr = fopen(fileName,"w");
    if(img->type == 1)
        fprintf(fptr, "%s", "P5\n");
    else
        fprintf(fptr, "%s", "P6\n");
    fprintf(fptr, "%d %d\n", img->width, img->height);
    fprintf(fptr, "%d\n", img->maxVal);
    for(int i = 0; i < img->height; i++){
        for(int j = 0 ; j < img->width; j++){
            for(int k = 0 ; k < img->type; k++)
                fprintf(fptr,"%c",img->pixels[i][j].val[k]);
        }
    }
    fclose(fptr);
}

//functions applys filter in dependency of filter type and process id
void apply_filter(image* img, char* fltr){
    
    float filter[3][3];
    pixel **pixels;
    
    pixels = (pixel **)malloc(sizeof(pixel *)*img->height);
    for(int i = 0; i < img->height; i++)
        pixels[i] = (pixel *)malloc(sizeof(pixel)*img->width);

    for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) filter[i][j] = zero[i][j];
    if(!strcmp("smooth",fltr))
        for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) filter[i][j] = smooth[i][j];
    if(!strcmp("blur",fltr))
        for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) filter[i][j] = blur[i][j];
    if(!strcmp("sharpen",fltr))
        for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) filter[i][j] = sharpen[i][j];
    if(!strcmp("mean",fltr))
        for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) filter[i][j] = mean[i][j];
    if(!strcmp("emboss",fltr))
        for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) filter[i][j] = emboss[i][j];

    for(int i = MAX(0, start - (id != 0 ? padding : 0)); i < stop + (id != (numtasks - 1) ? padding : 0); i++)
        for(int j = 0 ; j < img->width; j++)
            for(int k = 0 ; k < img->type; k++){
                //apply filter for poz i j for every channel
                float sum = 0;
                for(int row = 0; row < 3; row++)
                    for(int col = 0; col < 3; col++){
                        int rw = row - 1 + i;
                        int cl = col - 1 + j;
                        if(rw >= img->height || cl >= img->width || cl < 0 || rw < 0)
                            sum += 0;
                        else{
                            float fctr =  img->pixels[rw][cl].val[k];
                            sum += filter[row][col]*fctr;
                        }
                    }
                //clip sum to range 0-255
                if(sum >= 255.0f)
                    pixels[i][j].val[k] = (unsigned char)255;
                else
                if(sum <= 0.0f)
                    pixels[i][j].val[k] = (unsigned char)0;
                else
                    pixels[i][j].val[k] = (unsigned char)sum;

            }
    for(int i = 0; i < img->height; i++)
        for(int j = 0 ; j < img->width; j++)
            for(int k = 0 ; k < img->type; k++)
                img->pixels[i][j].val[k] = pixels[i][j].val[k];

    for(int i = 0; i < img->height; i++)
        free(pixels[i]);
    free(pixels);

}

int main (int argc, char *argv[]){

    padding = argc + 10;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);    
    
    //get image
    image *img = read_image(argv[1]);
    start = id*ceil(img->height/numtasks);
	stop = (id+1)*ceil(img->height/numtasks);
    if(id == numtasks - 1)
        stop = img->height;
        
    //apply filters
    for(int i = 3; i < argc; i++)
        apply_filter(img, argv[i]);
    
    //gather all info from other processes
    if(id == 0){
        for(int j = 1; j < numtasks; j++){
            start = j*ceil(img->height/numtasks);
	        stop = (j+1)*ceil(img->height/numtasks);
            if(j == numtasks - 1)
                stop = img->height;
            for(int i = start; i < stop; i++)
                MPI_Recv(img->pixels[i], img->width*3, MPI_UNSIGNED_CHAR, j, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        write_image(argv[2], img);
    }else{
        printf("%d %d \n", stop, img->height);
        for(int i = start; i < stop; i++)
            MPI_Send(img->pixels[i], img->width*3, MPI_UNSIGNED_CHAR, 0, i, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}