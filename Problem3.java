public class Problem3{
public static boolean isPrime(int num){
    if(num==1 || num==2)
        return false;
    for (x=2;x<num/2;x++){
        if(num%x==0)
            return false;
	}
    return true;
}
    
public static void main(String[] args){
	int highest_prime_factor=-1;
	int number_to_find=600851475143;

	for (x=1;x<number_to_find/2;x++){
		if(isPrime(x)){
			if(number_to_find%x==0){
				highest_prime_factor=x;
			}
		}
	}
	System.out.print(highest_prime_factor);
}
}