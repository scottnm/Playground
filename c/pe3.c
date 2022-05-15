#include <stdio.h>

#define LARGE_NUM 600851475143l

int largest_prime();

int main(){
    printf("%d\n", largest_prime());
}

int largest_prime(){
    int factor = 3;
    long large_num = LARGE_NUM;


    while(factor < large_num){
        if(large_num%factor == 0){
            large_num /= factor;
            factor--;
        }
        factor++;
    }

    return factor;
}
