#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define FILE_IO 4
#define FILE_I 3
#define STDIN_AS_I 2
#define ERR 1
#define LINE_SIZE 200
#define LOG 1
#define INITIAL_ARR_SIZE 1000
FILE *log_file;

struct pair{
    char *key;
    char *val;
};

struct mp{
    struct pair *lst;
    int size, cnt;
};

void init_mp(struct mp *map){
    map->size = INITIAL_ARR_SIZE;
    map->cnt = 0;
    map->lst = (struct pair*) malloc(map->size * sizeof(struct pair));
    for(int i = 0; i < map->size; i++){
        map->lst[i].key = NULL;
        map->lst[i].val = NULL;
    }
}

struct pair *get(struct mp *map, char *key){
    for(int i = 0 ; i < map->size; i++){
        if(strcmp(map->lst[i].key,key) == 0){
            return &(map->lst[i]);
        }
    }
    return NULL;
}

void set(struct mp *map, char *key, char *value){
    if(map->cnt + 1 >= map->size){
        map->lst = (struct pair *)realloc(map->lst, (map->size * 2) * sizeof(struct pair));
        for(int i = map->size; i < map->size * 2; i++){
            map->lst[i].key = NULL;
            map->lst[i].val = NULL;
        }
        map->size *= 2;
    }else{
        int key_exists = 0;
        for(int i = 0 ; i < map->cnt; i++){
            if(strcmp(map->lst[i].key, key) == 0){
                strcpy(map->lst[i].val, value);
                key_exists = 1;
            }
        }
        if(!key_exists){
            map->lst[map->cnt].key = (char *)malloc(sizeof(char) * LINE_SIZE);
            map->lst[map->cnt].val = (char *)malloc(sizeof(char) * LINE_SIZE);
            strcpy(map->lst[map->cnt].key, key);
            strcpy(map->lst[map->cnt].val, value);
            map->cnt++;
        }
    }
}


struct matrx{
    int size, cnt;
    char **mtrx;
};

void init_mtrx(struct matrx *input){
    
    input->size = INITIAL_ARR_SIZE;
    input->cnt = 0;
    input->mtrx = (char **)malloc(input->size * sizeof(char *));     

    for(int i = 0; i < input->size; i++){
        input->mtrx[i] = NULL;
        input->mtrx[i] = (char*)malloc(LINE_SIZE * sizeof(char));
    }
}

void add_line(struct matrx *input, char* buff){
    if(input->size <= input->cnt + 1){
        *(input->mtrx) = (char* )realloc(input->mtrx, (input->size*2)*sizeof(char*));
        for(int i = input->size; i < input->size*2; i++){
            input->mtrx[i] = NULL;
            input->mtrx[i] = (char*)malloc(LINE_SIZE * sizeof(char));
        }
        input->size *= 2;
    }else{
        memcpy(input->mtrx[input->cnt], buff, LINE_SIZE*sizeof(char));
        input->cnt++;
    }
}


int parse_argv(int argc, char *argv[], char **in, char **out, struct mp *map)
{
    for(int i = 0 ; i < argc; i++){
        fprintf(log_file, "%s ", argv[i]);
    }
    fprintf(log_file, "\n");
    
    if (argc == 1)
        return STDIN_AS_I;
    
    int i = 1;
    while(i < argc && argv[i][0] ==  '-' && argv[i][1] ==  'D') {
        if(strlen(argv[i]) >= 3){

        }else{
            i++;
            
        }
        //TODO: if not = retrn 1
        //printf("%s\n", argv[i]);
        i++;
    }

    while(i < argc && argv[i][0] ==  '-' && argv[i][1] ==  'I') {
        i++;
        //TODO: if not = retrn 1
        //printf("%s\n", argv[i]);
        i++;
    }

    if(i >= argc)
        return STDIN_AS_I;

    if(argv[i][0] == '-' && strlen(argv[i]) < 3)
        return ERR;
    if(i + 2 == argc) {
        int parm_sz = strlen(argv[i]) + 1;
        if(parm_sz < strlen(argv[i+1]) + 1){
            parm_sz = strlen(argv[i+1]) + 1;
        }
        *in = (char *)malloc(sizeof(char) * parm_sz);
        *out = (char *)malloc(sizeof(char) * (parm_sz));
        strcpy(*in, argv[i]);
        strcpy(*out, argv[i+1]);
        return FILE_IO;
    }else {
        if(i + 1 == argc){
            int parm_sz = strlen(argv[i]) + 1;
            *in = (char *)malloc(sizeof(char) * parm_sz);
            strcpy(*in, argv[i]);
            return FILE_I;
        }
    }
    return ERR;
}

void get_input(int code, struct matrx *input, char *in_file_path)
{
    FILE *file_in = NULL;
    if(code == FILE_I || code == FILE_IO){
        file_in = fopen(in_file_path, "r");
    }else
    {
        file_in = stdin;
    }
    
    char buff[LINE_SIZE];
    while(fgets(buff, sizeof(buff), file_in))
        add_line(input,buff);

    if(code == FILE_I || code == FILE_IO)
        fclose(file_in);
}

void print_out(struct matrx *data, char* out_file_path, int ret){
    
    FILE *out_file= NULL;
    
    if(ret == FILE_IO)
        out_file = fopen(out_file_path, "w");
    
    for (int i = 0 ; i < data->cnt; i++)
        if(ret == FILE_IO)
            fprintf(out_file, "%s", data->mtrx[i]);
        else
            printf("%s", data->mtrx[i]);
        
    if(ret == FILE_IO)
        fclose(out_file);
}

int main (int argc, char *argv[])
{
    log_file = fopen("log.txt", "a");
    
    char *in_file_path = NULL, *out_file_path = NULL;
    struct mp map;
    struct matrx input;
    
    init_mp(&map);
    init_mtrx(&input);
    
    int ret = parse_argv(argc, argv, &in_file_path, &out_file_path, &map);
    
    if (ret == ERR)
        return 1;

    get_input(ret, &input, in_file_path);
    print_out(&input, out_file_path, ret);


    if (out_file_path != NULL)
        free (out_file_path);
        
    if(in_file_path != NULL)
        free (in_file_path);

    if(input.mtrx != NULL){
        for (int i = 0; i < input.size; i++)
            free(input.mtrx[i]);
        free(input.mtrx);
    }

    if(map.lst != NULL){
        for(int i = 0; i < map.size; i++){
            if(map.lst[i].key != NULL)
                free(map.lst[i].key);
            if(map.lst[i].val != NULL)
                free(map.lst[i].val);
        }
        free(map.lst);
    }

    fclose(log_file);

    return 0;
}