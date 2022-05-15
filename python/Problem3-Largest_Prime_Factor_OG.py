def isPrime(num):
    if(num==1 or num==2):
        return False
    for x in range(2, num//2+1):
        if(num%x==0):
            return False
    return True

def my_range(start, end, step):
    while start <= end:
        yield start
        start += step    
    
highest_prime_factor=-1
number_to_find=600851475143

for x in my_range(1, number_to_find//2, 2):
    print(x)
    if(isPrime(x)):
        if(number_to_find%x==0):
            print(x)
            highest_prime_factor=x
print(highest_prime_factor)
