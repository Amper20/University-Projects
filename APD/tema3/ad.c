#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MASTER 0
#define MAX_LENGTH 50
#define IMAGE_TYPE_LENGTH 3

int numtasks, rank, len;
char *imageType;
int width, height;
int maxVal;

int start, end;

struct Cell *pixels;
struct Cell *pixelsAux;

struct Cell
{
   unsigned char red;
   unsigned char green;
   unsigned char blue;
};

float smooth[9] = {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0};
float blur[9] = {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0};
float sharpen[9] = {0.0, -2.0 / 3.0, 0, -2.0 / 3.0, 11.0 / 3.0, -2.0 / 3.0, 0, -2.0 / 3.0, 0.0};
float mean[9] = {-1.0, -1.0, -1.0, -1.0, 9.0, -1.0, -1.0, -1.0, -1.0};
float emboss[9] = {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0};

void readArguments(int argc, char *argv[], char *inputFileName, char *outputFileName, char *filters[])
{
   strcpy(inputFileName, argv[1]);
   strcpy(outputFileName, argv[2]);

   for (int i = 0; i < argc - 3; i++)
   {
      filters[i] = argv[i + 3];
   }
}

void readInputImage(char *inputFileName)
{
   FILE *fptr;
   if ((fptr = fopen(inputFileName, "r")) == NULL)
   {
      exit(1);
   }

   fscanf(fptr, "%s", imageType);

   char *uselessComment = malloc(MAX_LENGTH);
   for (int i = 0; i < 8; i++)
   {
      fscanf(fptr, "%s", uselessComment);
   }

   fscanf(fptr, "%d", &width);
   fscanf(fptr, "%d", &height);
   fscanf(fptr, "%d", &maxVal);
   unsigned char newLine;
   fscanf(fptr, "%c", &newLine);

   pixels = (struct Cell *)malloc(width * height * sizeof(struct Cell));

   for (int i = 0; i < width * height; i++)
   {
      fscanf(fptr, "%c", &pixels[i].red);
      if (strcmp(imageType, "P6") == 0)
      {
         fscanf(fptr, "%c", &pixels[i].green);
         fscanf(fptr, "%c", &pixels[i].blue);
      }
   }

   fclose(fptr);
}

unsigned char clamp(float value)
{
   if (value >= 255.0f)
   {
      return (unsigned char)255;
   }
   if (value <= 0.0f)
   {
      return (unsigned char)0;
   }
   return (unsigned char)value;
}

int inBoundaries(int row, int column, int width, int height)
{
   if (row >= 0 && row < height && column >= 0 && column < width)
      return 1;
   return 0;
}

void applyFilter(float filter[])
{
   // update pixels to store the values obtained from the previous filter
   for (int idx = start, auxIdx = 0; idx < end; idx++, auxIdx++)
   {
      pixels[idx].red = pixelsAux[auxIdx].red;
      if (strcmp(imageType, "P6") == 0)
      {
         pixels[idx].green = pixelsAux[auxIdx].green;
         pixels[idx].blue = pixelsAux[auxIdx].blue;
      }
   }

   for (int idx = start, auxIdx = 0; idx < end; idx++, auxIdx++)
   {
      int row = idx / width;
      int column = idx % width;
      int idxFilter = 0;
      float newRed = 0.0f;
      float newGreen = 0.0f;
      float newBlue = 0.0f;

      for (int xIdx = 0; xIdx < 3; xIdx++)
      {
         for (int yIdx = 0; yIdx < 3; yIdx++)
         {
            int newColumn = column + yIdx - 1;
            int newRow = row + xIdx - 1;
            if (inBoundaries(newRow, newColumn, width, height))
            {
               int newIdx = newRow * width + newColumn;
               float redFloat = (float)pixels[newIdx].red;
               newRed += filter[8 - idxFilter] * redFloat;
               if (strcmp(imageType, "P6") == 0)
               {
                  float greenFloat = (float)pixels[newIdx].green;
                  newGreen += filter[8 - idxFilter] * greenFloat;
                  float blueFloat = (float)pixels[newIdx].blue;
                  newBlue += filter[8 - idxFilter] * blueFloat;
               }
            }
            idxFilter++;
         }
      }

      // write new values to pixelsAux
      pixelsAux[auxIdx].red = clamp(newRed);
      if (strcmp(imageType, "P6") == 0)
      {
         pixelsAux[auxIdx].green = clamp(newGreen);
         pixelsAux[auxIdx].blue = clamp(newBlue);
      }
   }
}

void writeOutputImage(char *outputFileName)
{
   FILE *fptr;
   if ((fptr = fopen(outputFileName, "w")) == NULL)
   {
      exit(1);
   }

   fprintf(fptr, "%s\n", imageType);
   fprintf(fptr, "%d %d\n", width, height);
   fprintf(fptr, "%d\n", maxVal);

   for (int i = 0; i < height * width; i++)
   {
      fprintf(fptr, "%c", pixels[i].red);
      if (strcmp(imageType, "P6") == 0)
      {
         fprintf(fptr, "%c", pixels[i].green);
         fprintf(fptr, "%c", pixels[i].blue);
      }
   }
   fclose(fptr);
}

int main(int argc, char *argv[])
{
   char hostname[MPI_MAX_PROCESSOR_NAME];
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Get_processor_name(hostname, &len);

   imageType = malloc(IMAGE_TYPE_LENGTH);
   char *inputFileName = malloc(MAX_LENGTH);
   char *outputFileName = malloc(MAX_LENGTH);
   char *filters[argc - 3];
   // read run arguments
   readArguments(argc, argv, inputFileName, outputFileName, filters);
   // read input data
   readInputImage(inputFileName);

   int n = width * height;
   start = rank * ceil((double)n / numtasks);
   end = fmin((rank + 1) * ceil((double)n / numtasks), n);

   // resize the part of the image, if possible
   if (start > (width * 30))
   {
      start -= width * 30;
   }
   else
   {
      start = 0;
   }
   if (end + (width * 30) < width * height)
   {
      end += width * 30;
   }
   else
   {
      end = width * height;
   }

   pixelsAux = (struct Cell *)malloc((end - start + 1) * sizeof(struct Cell));
   // copy to pixelsAux initial values
   int auxIdx;
   for (int idx = start, auxIdx = 0; idx < end; idx++, auxIdx++)
   {
      pixelsAux[auxIdx].red = pixels[idx].red;
      if (strcmp(imageType, "P6") == 0)
      {
         pixelsAux[auxIdx].green = pixels[idx].green;
         pixelsAux[auxIdx].blue = pixels[idx].blue;
      }
   }

   // apply filters
   int filtersCounter = argc - 3;
   for (int filterIdx = 0; filterIdx < filtersCounter; filterIdx++)
   {
      if (strcmp(filters[filterIdx], "smooth") == 0)
      {
         applyFilter(smooth);
      }
      else if (strcmp(filters[filterIdx], "blur") == 0)
      {
         applyFilter(blur);
      }
      else if (strcmp(filters[filterIdx], "sharpen") == 0)
      {
         applyFilter(sharpen);
      }
      else if (strcmp(filters[filterIdx], "mean") == 0)
      {
         applyFilter(mean);
      }
      else if (strcmp(filters[filterIdx], "emboss") == 0)
      {
         applyFilter(emboss);
      }
   }

   int size;
   if (rank != MASTER)
   {
      size = end - start + 1;
      MPI_Send(&size, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
      MPI_Send(pixelsAux, sizeof(struct Cell) * size, MPI_UNSIGNED_CHAR, MASTER, 0, MPI_COMM_WORLD);
   }
   else
   {
      // copy the values computed by the master process
      start = rank * ceil((double)n / numtasks);
      end = fmin((rank + 1) * ceil((double)n / numtasks), n);

      for (int i = start, auxIdx = 0; i < end; i++, auxIdx++)
      {
         pixels[i].red = pixelsAux[auxIdx].red;
         if (strcmp(imageType, "P6") == 0)
         {
            pixels[i].green = pixelsAux[auxIdx].green;
            pixels[i].blue = pixelsAux[auxIdx].blue;
         }
      }

      // recv and copy the values computed by the remaining processes
      for (int process = 1; process < numtasks; process++)
      {
         MPI_Recv(&size, 1, MPI_INT, process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         free(pixelsAux);
         pixelsAux = (struct Cell *)malloc(sizeof(struct Cell) * size);
         MPI_Recv(pixelsAux, sizeof(struct Cell) * size, MPI_UNSIGNED_CHAR, process, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

         start = process * ceil((double)n / numtasks);
         end = fmin((process + 1) * ceil((double)n / numtasks), n);

         int aux = 0;
         if (start > width * 30)
         {
            aux += (width * 30);
         }

         printf("%d %d\n", start, end);
         for (int i = start, auxIdx = aux; i < end; i++, auxIdx++)
         {
            pixels[i].red = pixelsAux[auxIdx].red;
            if (strcmp(imageType, "P6") == 0)
            {
               pixels[i].green = pixelsAux[auxIdx].green;
               pixels[i].blue = pixelsAux[auxIdx].blue;
            }
         }
      }
      // write data
      writeOutputImage(outputFileName);
   }
   MPI_Finalize();
}