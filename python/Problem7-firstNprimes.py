def isPrime(num):
    if(num==1):
        return False
    for x in range(2, num//2+1):
        if(num%x==0):
            return False
    return True

count=2
number=3
while(count<10001):
    number+=2
    if(isPrime(number)):
        print(str(count)+" "+str(number))
        count+=1
print(number)    
        
