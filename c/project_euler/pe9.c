#include <stdio.h>

int isTrip(int a, int b, int c);

int findTrip();

int main()
{
	printf("%d\n", findTrip());
}

int findTrip(){
	int a,b,c;
	for(a = 1; a<500; a++){
		for(b = 500-a; (a+b)<1000; b++){
			int c = 1000-a-b;
			if(c > a && c > b && isTrip(a, b, c)){
				printf("%d\t%d\t%d\n", a,b,c);
				return a*b*c;
			}
				
		}
	}
	return 0;
}

int isTrip(int a, int b, int c){
	return (c*c) == ((a*a)+(b*b));
}
