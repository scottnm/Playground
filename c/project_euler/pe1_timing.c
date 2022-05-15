#include <stdio.h>
#include <time.h>

int sum1();
int sum2();

int main(){
    sum1(10, 23);
    sum2(10, 23);
}

int sum1(int start_n, int start_sum){
    int sum = start_sum;
    int n = start_n;
    time_t start;
    time_t end;
    time(&start);
    while(n<1000000000){
        int test1 = (n%5 == 0);
        int test2 = (n%3 == 0);
        if(test1 | test2){
            sum+=n;
        }
        n++;
    }
    time(&end);
    printf("With conditional moves: %f\n", difftime(end, start));

    printf("%d\n", sum);

    return sum;
}

int sum2(int start_n, int start_sum){
    int sum = start_sum;
    int n = start_n;
    time_t start;
    time_t end;
    time(&start);
    while(n<1000000000){
        if(n%5 == 0) sum+=n;
        else if(n%3 == 0) sum+=n;
        n++;
    }
    time(&end);
    printf("With conditional branches: %f\n", difftime(end, start));

    printf("%d\n", sum);

    return sum;
}
