#include <stdio.h>

//largest product of 2 3digit nums
#define LARGE_PROD 998001
//largest 3digit num
#define LARGE_FAC 999

int largest_palindrome();
int reverse_number();

int main(){
    printf("%d\n", largest_palindrome());
}

int largest_palindrome(){
    int i;
    int largest_prod = 0;
    //start at the top and work backwards because
    //we are finding the LARGEST product
    for(i = LARGE_FAC; i > 99; i--){
        int j;
        for(j = LARGE_FAC; j > 99; j--){

            int prod = i*j;
            if(prod == reverse_number(prod) && prod > largest_prod)
                largest_prod=prod;
        }
    }
    return largest_prod;
}

int reverse_number(int num){

    //error checking for invalid numbers
    if(num <= 0)
        return -1;

    int max_digit = 100000;

    //find largest digit's place
    while(num/max_digit == 0) max_digit/=10;

    //reverse digits
    int i;
    int new_num = 0;

    for(i = 1; i <= max_digit; i*=10){
        int curr_place = max_digit/i;
        new_num+=((num/curr_place) * i);
        num=num%curr_place;
    }

    return new_num;
}
