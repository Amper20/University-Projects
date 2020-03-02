#include <stdio.h>
#include <string.h>

int main(){
    char text[5120], word[5120];
    char words[100][5120];
    int n;
    fgets(text, 5120, stdin);
    scanf("%d", &n);
    for(int i = 0 ; i < n; i++){
        scanf("%s", words[i]);
    }
    
    for (int k = 0; k < n; k++){
        for(int i = 0; i < strlen(text); i++){
            
            int check = 1;
            int j = 0;
            
            while(j < strlen(words[k]) && j + i < strlen(text)){
                if(words[k][j] != text[i+j])
                    check = 0;
                j++;
            }

            if(check && j + i < strlen(text)){
                for(int j = 0 ; j < strlen(words[k]); j++)
                    text[i+j] = '*';
            }

        }
    }
    printf("%s", text);
    return 0;
}