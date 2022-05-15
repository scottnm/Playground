def sumOfSquares(x):
    sum=0
    for y in range(1,x+1):
        sum+=y**2
    return sum

def squareOfSum(x):
    sum=0
    for y in range(1,x+1):
        sum+=y
    return sum**2

def main(top_val):
    print (squareOfSum(top_val))
    print (sumOfSquares(top_val))
    return squareOfSum(top_val)-sumOfSquares(top_val)
    

print(main(100))
