#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define FILE_IO 4
#define FILE_I 3
#define STDIN_AS_I 2
#define ERR 1
#define BUFF_SIZE 200

struct pair{
    char *key;
    char *value;
};

struct hmp{
    struct pair *lst;
    int size;
};

struct pair *get(struct hmp *map, char *key){
    for(int i = 0 ; i < map->size; i++){
        if(strcmp(map->lst[i].key,key) == 0){
            return &(map->lst[i]);
        }
    }
    return NULL;
}

void set(struct hmp *map, char *key){
    
}

int parse_argv(int argc, char *argv[], char *in, char* out)
{
    // printf(">>>>>>>>>>>>>");
    // for(int i = 0 ; i < argc; i++){
    //     printf("%s ", argv[i]);
    // }
    // printf("<<<<<<\n");
    
    if (argc == 1)
        return STDIN_AS_I;
    
    int i = 1;
    while(i < argc && strcmp(argv[i], "-D") == 0){
        i++;
        //TODO: if not = retrn 1
        //printf("%s\n", argv[i]);
        i++;
    }

    while(i < argc && strcmp(argv[i], "-I") == 0){
        i++;
        //TODO: if not = retrn 1
        //printf("%s\n", argv[i]);
        i++;
    }

    if(i >= argc)
        return STDIN_AS_I;

    if(argv[i][0] == '-' && strlen(argv[i]) < 3)
        return ERR;
    printf("%d ", i);
    if(i + 2 == argc) {
        in = malloc(sizeof(char) * strlen(argv[i]));
        out = malloc(sizeof(char) * strlen(argv[i+1]));
        strcpy(in, argv[i]);
        strcpy(out, argv[i+1]);
        return FILE_IO;
    }else {
        if(i + 1 == argc){
            in = malloc(sizeof(char) * strlen(argv[i]));
            strcpy(in, argv[i]);
            return FILE_I;
        }
    }
    return ERR;
}

int get_input(int code, char ***input){
    int fd = fileno(stdin);
    if(code == FILE_I || code == FILE_IO){
        //open file
    }
    int lines = 1;
    *input = (char **)malloc(lines * sizeof(char *)); 
    for (int i = 0; i < lines; i++) 
        (*input)[i] = (char *)malloc(BUFF_SIZE * sizeof(int));
    char buffer[BUFF_SIZE];
    while(read(fd, buffer, BUFF_SIZE)){
        *input = (char**)realloc(*input, (lines + 1)*sizeof(char*));
        (*input)[lines] = (char*)malloc(BUFF_SIZE * sizeof(char));
        memcpy((*input)[lines], buffer, BUFF_SIZE * sizeof(char));
        lines++;
    }
    return lines;
}

int main (int argc, char *argv[])
{
    char *in_file, *out_file;
    char **input;
    int ret = parse_argv(argc, argv, in_file, out_file);
    int lines = get_input(ret, &input);
    for(int i = 0 ; i < lines; i++){
        printf("%s", input[i]);
    }
    return 0;
}