def isPalindrome(x):
    for y in range(0, len(x)//2):
        if(x[y]!=x[len(x)-1-y]):
            return False
    return True

largest_palindrome=0;

for x in range(100, 999):
    for y in range(x, 999):
        if (isPalindrome(str(x*y)) and (x*y)>largest_palindrome):
            largest_palindrome=x*y

print (largest_palindrome)
