#include <stdio.h>

#define FIB_MAX 4000000

int fibsum1();

int main(){
    printf("sum: %d\n", fibsum1());
}

int fibsum1(){
    //initialize terms
    int term1 = 1;
    int term2 = 2;

    //sum starts at 2 because initial term2 is 2
    int sum = 2;

    while(term2 < FIB_MAX){
        int i;

        //every 3rd fib term is even
        //..3, 5, ""8"", 13, 21, ""34""...
        for(i = 0; i < 3; i++){
            int temp = term2;
            term2 += term1;
            term1 = temp;
        }
        sum+=term2;
    }

    if(term2 > FIB_MAX) sum-=term2;

    return sum;
}
