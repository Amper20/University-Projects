#include<stdio.h>

int main(){
    double area = 0, delta = 0.00005;
    double quarterArea = 0;
    for(double x = 0 ; x <= 1; x += delta){
        for(double y = 0 ; y <= 1; y += delta){
            if(x*x+y*y <= 1){
                quarterArea += delta*delta;
            }
            area += delta*delta;
        }
    }
    printf("PI = %f \n", (4.0*quarterArea)/area);
}