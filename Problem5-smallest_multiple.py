
def is20Multiple(x):
    if(x==0):
        return False
    for y in range(1,20):
        if(x%y!=0):
            return False
    return True;

def my_range(start, end, step):
    while start <= end:
        yield start
        start += step

def findSmallestMultiple(start_val):
    iterator=start_val
    while(True):
        print(iterator)
        if(is20Multiple(iterator)):
            return iterator
        else:
            iterator+=20
        
print(findSmallestMultiple(2520))

"""
2 - 2,4,6,8,0,2,4,6,8,0 20
3 - 3,6,9,2,5,8,1,4,7,0 30
4 - 4,8,2,6,0,2,4,6,8,0 40
5 - 5,0
6 - 6,2,8,4,0,2,4,6,8,0 60
7 - 7,4,1,8,5,2,9,6,3,0 70
8 - 8,6,4,2,0,2,4,6,8,0 80
9 - 9,8,7,6,5,4,3,2,1,0 90
11- 1,2,3,4,5,6,7,8,9,0 110
13- 
"""
