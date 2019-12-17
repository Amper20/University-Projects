/*Butmalai Dan 333CC*/
%{ 
#include <stdio.h>
#include <ctype.h>
FILE *yyin;

typedef struct{
    char *text;
    int level;
    int complex;
} NOD;

int poz_node = 0, size = 0, open = 0, cnt_complex = 0; 
char *ans_complex, *ans_simple;
int ans_complex_size = 0, poz_complex = 0;
int ans_simple_size = 0, poz_simple = 0;

NOD *node;

%} 
  
%% 

"{"      ++open;
"}"      --open;  
.       ; 
\n      ;

Object.* { 
    //salvam linia care a dat match
    if(poz_node + 1 >= size ){
        //realocam arrayul de noduri in caz ca urmatoul nod va face overflow
        size *= 2;
        node = (NOD*)realloc(node, sizeof(NOD) * size);
    }
    node[poz_node].level = open;
    node[poz_node].complex = 0;
    //alocam strict cat e necesar
    node[poz_node].text = malloc(sizeof(char) * strlen(yytext));
    strcpy (node[poz_node].text, yytext);
    node[poz_node].text[strlen(yytext)-1] = '\0';
    poz_node++;
}

%% 
  
int yywrap(){} 

//afiseaza structura arborescenta din fisier
void disp(int i){
    int initial_lvl = node[i].level;
    if(i+1 < poz_node && node[i+1].level > initial_lvl){
        printf( "%s\n", node[i].text);
        i++;
        for(; i < poz_node; i++){
            if(node[i].level <= initial_lvl)
                break;
            for(int j = 0; j < node[i].level - initial_lvl; j++ )
                printf("\t");
            printf( "%s\n", node[i].text);
        }
    }
}

// genereaza doua stringuri de output ans_simple si ans_complex
void dispObj(int i, int complex){
    
    int prnt = 0;
    
    for(int j = 0; j < strlen(node[i].text); j++){
        if(prnt){
            //in dependenta de tipul obiectului il vom scrie in ans_simple sau ans_complex
            if(!complex){
                if(poz_simple + 1 >= ans_simple_size){
                    ans_simple_size *= 2;
                    ans_simple = (char*)realloc(ans_simple, sizeof(char) * ans_simple_size);
                }
                if(isprint(node[i].text[j]))
                    ans_simple[poz_simple] = node[i].text[j], poz_simple++;
                if(j == strlen(node[i].text) - 1)
                    ans_simple[poz_simple] = ',', poz_simple++;
            }else{
                if(poz_complex + 1 >= ans_complex_size){
                    ans_complex_size *= 2;
                    ans_complex = (char*)realloc(ans_complex, sizeof(char) * ans_complex_size);
                }
                if(isprint(node[i].text[j]))
                    ans_complex[poz_complex] = node[i].text[j], poz_complex++;
                if(j == strlen(node[i].text) - 1)
                    ans_complex[poz_complex] = ',', poz_complex++;
            }
        }
        prnt |= (node[i].text[j] == ' ' ? 1 : 0);
    }

}

int main(int argc, char **argv) { 

    yyin = fopen(argv[1], "r");
    
    size = 5;    
    node = (NOD*)malloc(sizeof(NOD) * size);

    ans_complex_size = ans_simple_size = 10;
    ans_complex = (char* )malloc(sizeof(char) * ans_complex_size);
    ans_simple = (char* )malloc(sizeof(char) * ans_simple_size);

    yylex(); 
    
    for(int i = 0; i < poz_node; i++)
        if(i+1 < poz_node && node[i+1].level > node[i].level){
            node[i].complex = 1;
            cnt_complex++;
        }

    for(int i = 0 ; i < poz_node; i++)
        dispObj(i, node[i].complex);

    ans_simple[poz_simple-1] = '\0';
    ans_complex[poz_complex-1] = '\0';

    if(poz_node - cnt_complex != 0){
        if(poz_node - cnt_complex  == 1)
            printf("%d obiect simplu: %s\n", poz_node - cnt_complex, ans_simple);
        else
            printf("%d obiecte simple: %s\n", poz_node - cnt_complex, ans_simple);
    }
        
    if(cnt_complex != 0){
        if(cnt_complex == 1)
            printf("%d obiect complex: %s\n", cnt_complex, ans_complex);
        else
            printf("%d obiecte complexe: %s\n", cnt_complex, ans_complex);

    }
        
    for(int i = 0; i < poz_node; i++)
        disp(i);

    //eliberam memoria ocupata
    for(int i = 0; i < poz_node; i++)
        free(node[i].text);
    free(node);
    free(ans_complex);
    free(ans_simple);
    
    fclose(yyin);

    return 0; 
} 