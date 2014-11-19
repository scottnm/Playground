//NAIVE SOLUTION
#define PRIME_TO_FIND 10001
int findPrime(int n);
int isPrime(int i);

int main(){
	printf("%d\n", findPrime(PRIME_TO_FIND));
	return;
}

int findPrime(int n){
	int count = 0;
	long i = 0;
	while(count<10001){
		i++;
		if(isPrime(i)){
			count++;
		}
	}
	return i;
}

int isPrime(int i){
	long num = i;
	while(i>1){
		if(num%i==0)
			return 0;
	}
	return 1;
}
