def isPythagoreanTriple(a,b,c):
    if(a**2+b**2==c**2):
        return True
    else:
        return False
def findTriple():
    for a in range(1,1001):
        for b in range(a,1001):
            for c in range(b,1001):
                print(str(a)+" "+str(b)+" "+str(c))
                if(a+b+c==1000):
                    if(isPythagoreanTriple(a,b,c)):
                        return a*b*c
findTriple()
