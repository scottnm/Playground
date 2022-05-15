term1=1
term2=2
temp=0
fib_sum=0;

while(term2<4000000):
    if(term2%2==0):
        fib_sum+=term2
    temp=term2
    term2+=term1
    term1=temp
    print(str(fib_sum)+" "+str(term2))

print(fib_sum)
    
