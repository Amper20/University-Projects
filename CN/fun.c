#include<stdio.h>

#define mod ((1<<16) + 1)

int char_to_int(unsigned char* val, int size){
    int ret = 0;
    for(int i = 0 ; i < size; i++){
        ret = ret << 1;
        ret += val[i] == 0 ? 0:1;
    }
    return ret;
}


void int_to_char(unsigned char* ret, int size, int val){
    for(int i = 0 ; i < size; i++, val/=2)
        ret[i] = val%2;
    for(int i = 0 ; i < (size)/2; i++){
        unsigned char aux = ret[i];
        ret[i] = ret[size - i - 1];
        ret[size - i - 1]  = aux;
    }
}

int add(unsigned char *a, unsigned char *b, int size_a, int size_b){
    int a_int = char_to_int(a, size_a);
    int b_int = char_to_int(b,size_b);
    return (a_int + b_int)%(mod);
}

int mul(unsigned char *a, unsigned char *b, int size_a, int size_b){
    int a_int = char_to_int(a, size_a);
    int b_int = char_to_int(b,size_b);
    return (a_int * b_int)%(mod);
}

void shr(unsigned char *arr, int size, int count){
    for(int i = 0 ; i < size; i++){
        arr[i] = (i + count < size) ? arr[i+count] : 0;
    }
}

void disp(unsigned char *arr, int size){
    for(int i = 0; i < size; i++)
        printf("%d ", (int)arr[i]);
    printf("\n");
}

int main(){

    unsigned char a[16] = {1,0,0,1,1,0,0,1,1,0,0,1,1,0,0}; //39320

    int size = 16;

    disp(a,size);
    //char_to_int
    printf("%d\n", char_to_int(a,size));

    //int_to_char + shr
    unsigned char test1[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int_to_char(test1, size, 39320);
    shr(test1, size, 7);
    disp(test1, size);
    printf("\n");
    //add mul
    unsigned char addd[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned char mull[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int ad = add(a,a,size,size);
    int ml = mul(a,a,size,size);
    int_to_char(addd, size, ad);
    int_to_char(mull, size, ml);
    printf("%d\n", ad);
    disp(addd, size);
    printf("%d\n", ml);
    disp(mull, size);

}