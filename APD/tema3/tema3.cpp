#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

typedef struct{
    unsigned char val[3];
} pixel;

typedef struct{
    int width, height;
    int type; //1 grayscale, 3 rgb
    int maxVal;
    pixel **pixels;
} image;

#define MAX_LEN 1000

char str[MAX_LEN];
int numtasks = 0, id = 0;
float smooth[3][3] = { {(float)1/9, (float)1/9, (float)1/9}, {(float)1/9, (float)1/9, (float)1/9}, {(float)1/9, (float)1/9, (float)1/9} };
float zero[3][3] = { {(float)0, (float)0, (float)0}, {(float)0, (float)1, (float)0}, {(float)0, (float)0, (float)0} };
float blur[3][3] = { {(float)1/16, (float)2/16, (float)1/16}, {(float)2/16, (float)4/16, (float)2/16}, {(float)1/16, (float)2/16, (float)1/16} };
float sharpen[3][3] = { {(float)0, (float)-2/3, (float)0}, {(float)-2/3, (float)11/3, (float)-2/3}, {(float)0, (float)-2/3, (float)0} };
float mean[3][3] = { {(float)-1, (float)-1, (float)-1}, {(float)-1, (float)9, (float)-1}, {(float)-1, (float)-1, (float)-1} };
float emboss[3][3] = { {(float)0, (float)-1, (float)0}, {(float)0, (float)0, (float)0}, {(float)0, (float)1, (float)0} };

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
    printf("%d %d %d %d\n", ret->width, ret->height, ret->type, ret->maxVal);
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

void init_filters(){
    
}

void apply_filter(image* img, char* fltr){
    
    float filter[3][3];
    pixel pixels[img->height][img->width];
    
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
    if(!strcmp("zero",fltr))
        for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) filter[i][j] = zero[i][j];
    
    for(int i = 0; i < 3; i++) for(int j = 0; j < 3; j++) cout << filter[i][j] << " ";
    cout << "\n";

    for(int i = 0; i < img->height; i++)
        for(int j = 0 ; j < img->width; j++){
            for(int k = 0 ; k < img->type; k++){
                float sum = 0;
                for(int row = 0; row < 3; row++){
                    for(int col = 0; col < 3; col++){
                        int rw = row - 1 + i;
                        int cl = col - 1 + j;
                        if(rw >= img->height || cl >= img->width || cl < 0 || rw < 0)
                            continue;
                        float fctr =  img->pixels[rw][cl].val[k];
                        sum += filter[row][col]*fctr;
                    }
                }
                if(sum >= 255.0f)
                    pixels[i][j].val[k] = (unsigned char)255;
                else
                if(sum <= 0.0f)
                    pixels[i][j].val[k] = (unsigned char)0;
                else
                    pixels[i][j].val[k] = (unsigned char)sum;

            }
        }
    for(int i = 0; i < img->height; i++){
        for(int j = 0 ; j < img->width; j++){
            for(int k = 0 ; k < img->type; k++)
                img->pixels[i][j].val[k] = pixels[i][j].val[k];
        }
    }
    
}

int main (int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&id);
    init_filters();
    if(id == 0){   
        image *img = read_image(argv[1]);
        for(int i = 3; i < argc; i++){
            apply_filter(img, argv[i]);
        }
        write_image(argv[2], img);
    }
    MPI_Finalize();
}