#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FILE_IO 4
#define FILE_I 3
#define STDIN_AS_I 2
#define ERR 1
#define LINE_SIZE 200
#define LOG 1
#define INITIAL_ARR_SIZE 1000
#define TYPE_D 1
#define TYPE_I 2
#define TYPE_DEL 3

struct pair
{
    char *key;
    char *val;
    int type;
};

struct mp
{
    struct pair *lst;
    int size, cnt;
};

/* initialize map structure */
void init_mp(struct mp *map)
{
    int i;
    map->size = INITIAL_ARR_SIZE;
    map->cnt = 0;
    map->lst = (struct pair*) malloc(map->size * sizeof(struct pair));
    for(i = 0; i < map->size; i++){
        map->lst[i].key = NULL;
        map->lst[i].val = NULL;
        map->lst[i].key = (char *)malloc(sizeof(char) * LINE_SIZE);
        map->lst[i].val = (char *)malloc(sizeof(char) * LINE_SIZE);
    }
}

/* check if key is present in map structure */
int get(struct mp *map, char *key, int type){
    int i;
    for(i = 0 ; i < map->cnt; i++)
        if(strcmp(map->lst[i].key,key) == 0 && map->lst[i].type == type)
            return 1;
    return 0;
}

/* set a corelation between key and value in map structure */
void set(struct mp *map, char *key, char *value, int type){
    int i;

    if(map->cnt + 1 >= map->size){
        map->lst = (struct pair *)realloc(map->lst, (map->size * 2) * sizeof(struct pair));
        
        for(i = map->size; i < map->size * 2; i++){
            map->lst[i].key = NULL;
            map->lst[i].val = NULL;
        }
        map->size *= 2;
    }else{
        int key_exists = 0;
        for(i = 0 ; i < map->cnt; i++){
            if(strcmp(map->lst[i].key, key) == 0 && map->lst[i].type == type){
                memcpy(map->lst[i].val, value, strlen(value) + 1);
                key_exists = 1;
            }
        }
        if(!key_exists){
            memcpy(map->lst[map->cnt].key, key, strlen(key) + 1);
            memcpy(map->lst[map->cnt].val, value, strlen(value) + 1);
            map->lst[map->cnt].type = type;
            map->cnt++;
        }
    }
}


struct matrx{
    int size, cnt;
    int *prnt;
    char **mtrx;
};

/* initialize matrix structure that keeps lines */
void init_mtrx(struct matrx *input){
    
    int i;
    
    input->size = INITIAL_ARR_SIZE;
    input->cnt = 0;
    input->mtrx = (char **)malloc(input->size * sizeof(char *));     
    input->prnt = (int *)malloc(input->size * sizeof(int));
    for(i = 0; i < input->size; i++){
        input->mtrx[i] = NULL;
        input->prnt[i] = 1;
        input->mtrx[i] = (char*)malloc(LINE_SIZE * sizeof(char));
    }
}

/* appends a line to input structure */
void add_line(struct matrx *input, char* buff){
    int i;
    if(input->size <= input->cnt + 1){
        *(input->mtrx) = (char* )realloc(input->mtrx, (input->size*2)*sizeof(char*));
        for(i = input->size; i < input->size*2; i++){
            input->mtrx[i] = NULL;
            input->mtrx[i] = (char*)malloc(LINE_SIZE * sizeof(char));
        }
        input->size *= 2;
    }else{
        memcpy(input->mtrx[input->cnt], buff, LINE_SIZE*sizeof(char));
        input->cnt++;
    }
}

/* parses parameters specified with D */
void parseD(struct mp *map, char*argv){

    char *buff_key, *buff_value;
    int j, cnt;
    size_t n;
    j = 0;
    cnt = 0;
    n = strlen(argv);
    buff_key = (char*)malloc((n + 1) * sizeof(char));
    buff_value = (char*)malloc((n + 1) * sizeof(char));
    for(; j < n && (argv[j] != '=' && argv[j] != ' '); j++, cnt++)
        buff_key[cnt] = argv[j];
    buff_key[cnt] = '\0';
    if(j + 1 >= n)
        buff_value[0] = '\0';
    else{
        j++;
        cnt = 0;
        for (;j<n;j++, cnt++){
            if(argv[j] == '\r' || argv[j] == '\n')
                break;
            buff_value[cnt] = argv[j];
        }
    }
    buff_value[cnt] = '\0';
    set(map, buff_key, buff_value, TYPE_D);
    free(buff_key);
    free(buff_value);
}

/* Parses parameters with an I as argument*/
void parseI(struct mp *map, char*argv){
    char *buff_value;
    int j;
    size_t n = strlen(argv);
    buff_value = (char*)malloc((n + 1) * sizeof(char));
    for(j = 0; j < n; j++)
        buff_value[j] = argv[j];
    buff_value[n] = '\0';
    set(map, buff_value, buff_value, TYPE_I);
    free(buff_value);
}

/* function that parses args */
int parse_argv(int argc, char *argv[], char **in, char **out, struct mp *map)
{
    int i;
    size_t parm_sz;
    
    if (argc == 1)
        return STDIN_AS_I;
    
    i = 1;
    while(i < argc && argv[i][0] ==  '-' && argv[i][1] ==  'D') {
        if(strlen(argv[i]) >= 3){
            parseD(map, argv[i] + 2);
        }else{
            i++;
             parseD(map, argv[i]);
        }
        i++;
    }

    while(i < argc && argv[i][0] ==  '-' && argv[i][1] ==  'I') {
        if(strlen(argv[i]) >= 3){
            parseI(map, argv[i] + 2);
        }else{
            i++;
             parseI(map, argv[i]);
        }
        i++;
    }

    if(i >= argc)
        return STDIN_AS_I;

    if(argv[i][0] == '-' && strlen(argv[i]) < 3)
        return ERR;
    if(i + 2 == argc) {
        
        parm_sz = strlen(argv[i]) + 1;

        if(parm_sz < strlen(argv[i+1]) + 1)
            parm_sz = strlen(argv[i+1]) + 1;
        
        *in = (char *)malloc(sizeof(char) * parm_sz);
        *out = (char *)malloc(sizeof(char) * (parm_sz));
        strcpy(*in, argv[i]);
        strcpy(*out, argv[i+1]);
        return FILE_IO;
    }else {
        if(i + 1 == argc){
            parm_sz = strlen(argv[i]) + 1;
            *in = (char *)malloc(sizeof(char) * parm_sz);
            strcpy(*in, argv[i]);
            return FILE_I;
        }
    }
    return ERR;
}

/* funtion that reads from FILE */
int get_input(int code, struct matrx *input, char *in_file_path)
{
    FILE *file_in = NULL;
    char buff[LINE_SIZE];

    if(code == FILE_I || code == FILE_IO)
        file_in = fopen(in_file_path, "r");
    else
        file_in = stdin;

    if (file_in == NULL)
        return ERR;

    while(fgets(buff, sizeof(buff), file_in))
        add_line(input,buff);

    if(code == FILE_I || code == FILE_IO)
        fclose(file_in);
    return 0;
}

int print_out(int ret, struct matrx *data, char* out_file_path){
    
    FILE *out_file= NULL;
    int i;

    if(ret == FILE_IO)
        out_file = fopen(out_file_path, "w");

    if (ret == FILE_IO && out_file == NULL)
        return ERR;

    for (i = 0 ; i < data->cnt; i++)
        if(data->prnt[i])
            if(ret == FILE_IO)
                fprintf(out_file, "%s", data->mtrx[i]);
            else
                printf("%s", data->mtrx[i]);
    
    if(ret == FILE_IO)
        fclose(out_file);
    return 0;
}

int check_var(char *bgn, char *en){
    int cnt = 0, i = 0;
    for(i = 0; bgn + i != en; i++)
        if(bgn[i] == '"')
            cnt += 1;
    return (cnt%2 == 0);
}

void free_mem(char *in_file_path, char *out_file_path, struct mp *map, struct matrx *input)
{
    int i; 
    if (out_file_path != NULL)
        free (out_file_path);
        
    if(in_file_path != NULL)
        free (in_file_path);

    if(input->mtrx != NULL){
        for (i = 0; i < input->size; i++)
            free(input->mtrx[i]);
        free(input->mtrx);
        free(input->prnt);
    }

    if(map->lst != NULL){
        for(i = 0; i < map->size; i++){
            if(map->lst[i].key != NULL)
                free(map->lst[i].key);
            if(map->lst[i].val != NULL)
                free(map->lst[i].val);
        }
        free(map->lst);
    }

}

void solve_define(struct mp *map, struct matrx *input, int *found_main, int *i, char *poz)
{    
    int last = 0, cnt, pass, j;
    char *multiple, *curr_line;
    char buff[LINE_SIZE] = {0};
    multiple = strchr(input->mtrx[*i], '\\');
    poz += strlen("#define ");
    
    for(cnt = 0; multiple != NULL || last; cnt++){
        input->prnt[*i] = 0;
        curr_line = (cnt == 0 ? poz : input->mtrx[*i]);
        pass = 0;
        for(j = 0 ; j < strlen(curr_line); j++){
            if(curr_line[j] == '\\' || curr_line[j] == '\n'){
                if(curr_line[j] == '\\' && curr_line[j-1] != ' ')
                    strncat(buff, " ", 1);
                break;
            }
            pass = pass || (curr_line[j] != ' ');
            if(pass)
                strncat(buff, curr_line + j, 1);
        }  
        (*i)++;
        multiple = strchr(input->mtrx[*i], '\\');
        if(last == 1)
            break;
        if(last == 0 && multiple == NULL)
            last = 1;        
    } 
    strncat(buff, "\0", 1);
    if(strlen(buff) != 0)
        parseD(map, buff);
    else
        parseD(map, poz);
    *found_main = 0;
}

void solve_if(struct matrx *input, int *i, char *poz)
{  
    int disp = 0;
    while(strstr(input->mtrx[*i], "#endif") == NULL){
        input->prnt[*i] = 0;
        if(!disp){
            poz = strstr(input->mtrx[*i], "if ");
            if(poz != NULL){
                poz += strlen("if ");
                if(((*poz) - '0')){
                    (*i)++;
                    for( ;strstr(input->mtrx[*i], "#") != NULL; (*i)++)
                        input->prnt[*i] = 1;
                    disp = 1;
                }
            }
            poz = strstr(input->mtrx[*i], "#else");
            if(poz != NULL){
                (*i)++;
                for( ;strstr(input->mtrx[*i], "#") != NULL; (*i)++)
                    input->prnt[*i] = 1;
                disp = 1;
            }
        }
        (*i)++;
    }
    input->prnt[*i] = 0;
    (*i)++;
}

void solve_ifdef(struct mp *map, struct matrx *input, int *i, char *poz)
{ 
    int j = 0;
    char buff[LINE_SIZE];
    poz = strstr(input->mtrx[*i], "#ifdef ");
    if(poz == NULL){

    }else{
        poz += strlen("#ifdef ");
        memcpy(buff, poz, strlen(poz));
        for(; j < strlen(poz); j++)
            if((buff[j] >= '9' || buff[j] <= '0') && (buff[j] >= 'Z' || buff[j] <= 'A') && (buff[j] >= 'z' || buff[j] <= 'a'))
                break;
        buff[j] = '\0';
        // printf("%d %s\n", get(map,buff,TYPE_D), buff);
        if(get(map,buff,TYPE_D)){
            input->prnt[*i] = 0;
            (*i)++;
            input->prnt[*i] = 1;
            (*i)++;
            input->prnt[*i] = 0;
            (*i)++;
        }
        else{
            input->prnt[*i] = 0;
            (*i)++;
            input->prnt[*i] = 0;
            (*i)++;
            input->prnt[*i] = 0;
            (*i)++;
        }
        
    }
}


void replace_define(struct mp *map, struct matrx *input)
{
    char *poz;
    int found_main = 1, pass = 1, i, j;
    char buff[LINE_SIZE];

    for(i = 0; i < input->cnt; i++){
        poz = strstr(input->mtrx[i], "#ifdef ");
        if(poz != NULL)
            solve_ifdef(map, input, &i, poz);
    }
    for(i = 0; i < input->cnt; i++){
        //check for #define
        poz = strstr(input->mtrx[i], "#define ");
        if(poz != NULL)
            solve_define(map, input, &found_main, &i, poz);
        //check for main
        poz = strstr(input->mtrx[i], "int main()");
        found_main = found_main || (poz != NULL);
        if (input->prnt[i] != 0)
            input->prnt[i] = found_main;
        
        //check for undef
        poz = strstr(input->mtrx[i],"#undef ");
        if(poz != NULL){
            poz += strlen("#undef ");
            j = 0;
            for(; poz[j] != '\r' && poz[j] != '\n'; j++);
            poz[j] = '\0';
            for(j = 0; j < map->cnt; j++)
                if(strcmp(map->lst[j].key, poz) == 0)
                    map->lst[j].type = TYPE_DEL;
            input->prnt[i] = 0;
        }

        //replace define
        pass = 1;
        while(pass){
            pass = 0;
            for(j = 0; j < map->cnt; j++){
                poz = strstr(input->mtrx[i], map->lst[j].key);
                if(poz != NULL && map->lst[j].type == TYPE_D && check_var(input->mtrx[i], poz) && input->prnt[i]){
                    buff[0] = '\0';
                    strncat(buff, input->mtrx[i], poz - input->mtrx[i]);
                    strncat(buff, map->lst[j].val, strlen(map->lst[j].val));
                    strncat(buff, poz + strlen(map->lst[j].key), strlen(poz) - strlen(map->lst[j].key));
                    strncat(buff, "\0", 1);
                    memcpy(input->mtrx[i], buff, LINE_SIZE);
                    pass = 1;
                }
            }
        }

    }
    for(i = 0; i < input->cnt; i++){
        poz = strstr(input->mtrx[i], "#if ");
        if(poz != NULL)
            solve_if(input, &i, poz);
    }
    
            
}

int main (int argc, char *argv[])
{    
    char *in_file_path = NULL, *out_file_path = NULL;
    struct mp map;
    struct matrx input;
    int ret;

    init_mp(&map);
    init_mtrx(&input);
    
    ret = parse_argv(argc, argv, &in_file_path, &out_file_path, &map);
    if (ret == ERR)
        return 1;

    ret = get_input(ret, &input, in_file_path);
    if (ret == ERR)
        return 1;

    replace_define(&map, &input);
    
    ret = print_out(ret, &input, out_file_path);
    if (ret == ERR)
        return 1;

    free_mem(in_file_path, out_file_path, &map, &input);

    return 0;
}