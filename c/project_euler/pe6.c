#include <stdio.h>

int sumofsquares(int limit);
int squareofsum(int limit);

int main(){
	printf("%d\n", squareofsum(100)-sumofsquares(100));
}

int sumofsquares(int limit){
	int sum = 1;
	int i;
	for(i = 2; i <= limit; i++){
		sum+= (i*i);
	}
	return sum;
}

int squareofsum(int limit){
	int sum = 0;
	int i;
	for(i = 1; i<=limit; i++){
		sum+=i;
	}
	return (sum*sum);
}