//NAIVE SOLUTION
#include <stdio.h>
#define PRIME_TO_FIND 10001
long long int findPrime(int n);
int isPrime(long long i);

int main(){
	printf("%lld\n", findPrime(PRIME_TO_FIND));
	return 0;
}

long long int findPrime(int n){
	int count = 0;
	long long i = 1;
	while(count<n){
		i++;
		//printf("%lld\n", i);
		if(isPrime(i)){
			count++;
			printf("%d\t%lld\n", count, i);
		}
	}
	return i;
}

int isPrime(long long i){
	long long num = i;
	i--;
	while(i>1){
		if(num%i==0)
			return 0;
		i--;
	}
	return 1;
}
