#include <stdio.h>

/*
Solved both with timing data using conditional
branching as well as conditional moving
in a separate file pe1_timing.c

With large values of n_max conditional branching
yielded faster results by a ratio of 4:5
*/

int main(){
    //problem gives initial sum and n
    int sum = 23;
    int n = 10;
    int n_max = 1000;

    //check for all values below 1000
    while(n < n_max){
        //update sum if divisible by 3 or 5
        if(n%5 == 0) sum+=n;
        else if(n%3 == 0) sum+=n;
        n++;
    }

    printf("%d\n", sum);
}
