#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct{
    unsigned char r, g, b;
} rgb;

rgb **img, **tmp;
int width, height, type;
float kernel[3][3];
int bgn, en;
int rank, N;

void readLoc(char *loc){

    FILE *file = fopen(loc,"r");

    char buf[500];

    fgets(buf, 500, file);  
    type = (buf[1] == '5');

    fgets(buf, 500, file);
    fgets(buf, 500, file);
    
    sscanf(buf, "%d %d", &width, &height);
    fgets(buf, 500, file);
    int garbage;
    sscanf(buf, "%d", &garbage);
    
    img = (rgb **)malloc(height * sizeof(rgb *));
    for(int i = 0; i < height; i++)
        img[i] = (rgb *)malloc(width * sizeof(rgb));
       
    tmp = (rgb **)malloc(height * sizeof(rgb *));
    for(int i = 0; i < height; i++)
        tmp[i] = (rgb *)malloc(width * sizeof(rgb));

    for(int i = 0; i < height; i++){
        for(int j = 0 ; j < width; j++){
            fscanf(file,"%c", &img[i][j].r);
            if(!type){
                fscanf(file,"%c", &img[i][j].g);
                fscanf(file,"%c", &img[i][j].b);
            }
        }
    }
}

void writeLoc(char *loc){

    FILE *file = fopen(loc,"w");
    
    if(type)
        fprintf(file, "%s%d %d\n%d\n", "P5\n", width, height, 255);
    else
        fprintf(file, "%s%d %d\n%d\n", "P6\n", width, height, 255);

    for(int i = 0; i < height; i++){
        for(int j = 0 ; j < width; j++){
            fprintf(file,"%c",img[i][j].r);
            if(!type){
                fprintf(file,"%c",img[i][j].g);
                fprintf(file,"%c",img[i][j].b);
            }
        }
    }
    fclose(file);
}

float sumup(int Y, int X, char channel){
    
    float ans = 0.0f;
    
    for(int y = 0; y < 3; y++){
        //printf("%f", ans);
        float lineAns = 0.0f;
        for(int x = 0; x < 3; x++){
            int newY = y + Y;
            int newX = x + X;
            //shift kernel
            newX--;
            newY--;
            if(newY < height && newX < width && newX >= 0 && newY >= 0){   
                float pixel = 0.0f;
                if(channel == 'r')
                    pixel =  (float)img[newY][newX].r;
                if(channel == 'g')
                    pixel =  (float)img[newY][newX].g;
                if(channel == 'b')
                    pixel =  (float)img[newY][newX].b;
                ans += kernel[y][x]*pixel;
            }
        }
        //printf("%f", lineAns);
        ans += lineAns;
    }
    if(ans > 255.0f)
        return 255.0f;

    if(ans < 0.0f)
        return 0.0f;

    return ans;
}

void applyKernel(){

    int bgn1 = bgn;
    int en1 = en;
    bgn1 -= 30;
    en1 += 30;
    bgn1 = ((bgn1 < 0) ? 0 : bgn1);
    en1 = (en1 > height ? height : en1);

    for(int i = bgn1; i < en1; i++)
        for(int j = 0 ; j < width; j++)
                if(type){
                    tmp[i][j].r =  sumup(i, j, 'r');
                }else{
                    tmp[i][j].r =  sumup(i, j, 'r');
                    tmp[i][j].g =  sumup(i, j, 'g');
                    tmp[i][j].b =  sumup(i, j, 'b');
                }             

    for(int i = 0; i < height; i++)
        for(int j = 0 ; j < width; j++)
                img[i][j] = tmp[i][j];
}

int main (int argc, char *argv[]){

    int padding = argc + 10;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&N);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);    

    readLoc(argv[1]);

    bgn = rank * ceil(height/N);
    if(rank == N - 1)
        en = height;
    else
	    en = (rank + 1) * ceil(height/N);

    for(int x = 3; x < argc; x++){
  
        if(!strcmp("smooth",argv[x])){
            float smooth[3][3] = { {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f}, {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f}, {1.0f/9.0f, 1.0f/9.0f, 1.0f/9.0f} };
            for(int i = 0; i < 3; i++)
                memcpy(&kernel[i], &smooth[i], sizeof(kernel[0]));
        }
        if(!strcmp("blur",argv[x])){
            float blur[3][3] = { {1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f}, {2.0f/16.0f, 4.0f/16.0f, 2.0f/16.0f}, {1.0f/16.0f, 2.0f/16.0f, 1.0f/16.0f} };
            for(int i = 0; i < 3; i++) 
                memcpy(&kernel[i], &blur[i], sizeof(kernel[0]));
        }
        
        if(!strcmp("sharpen",argv[x])){
            float sharpen[3][3] = { {0.0f, -2.0f/3.0f, 0.0f}, {-2.0f/3.0f, 11.0f/3.0f, -2.0f/3.0f}, {0.0f, -2.0f/3.0f, 0.0f} };
            for(int i = 0; i < 3; i++) 
                memcpy(&kernel[i], &sharpen[i], sizeof(kernel[0]));
        }

        if(!strcmp("mean",argv[x])){
            float mean[3][3] = { {-1.0f, -1.0f, -1.0f}, {-1.0f, 9.0f, -1.0f}, {-1.0f, -1.0f, -1.0f} };
            for(int i = 0; i < 3; i++)
                memcpy(&kernel[i], &mean[i], sizeof(kernel[0]));
        }

        if(!strcmp("emboss",argv[x])){
            float emboss[3][3] = { {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f} };
            for(int i = 0; i < 3; i++) 
                memcpy(&kernel[i], &emboss[i], sizeof(kernel[0]));
        }

        applyKernel();
    }
        
    if(rank){
        for(int i = bgn; i < en; i++)
            MPI_Send(img[i], width*3, MPI_UNSIGNED_CHAR, 0, i, MPI_COMM_WORLD);
    }else{
        for(int j = 1; j < N; j++){
            bgn = j * ceil(height/N);
            if(rank == N - 1)
                en = height;
            else
                en = (j + 1) * ceil(height/N);
            for(int i = bgn; i < en; i++)
                MPI_Recv(img[i], width*3, MPI_UNSIGNED_CHAR, j, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        writeLoc(argv[2]);
    }
    MPI_Finalize();
    return 0;
}