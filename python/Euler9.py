def specialPythagoreanTriplet():
    #digit must be between 1 and 1000
    for a in range(1,1000):
        """
        -assuming its a special triplet no a,b,c value will be the same
        -so we can assume a scenario where our b-value has to be greater than the a value
        """
        for b in range(a, 1000):
            #special rule of triangles, side c must be less than the sum of the other two sides
            for c in range(0, a+b):
                if(a+b+c==1000):
                    if((a*a)+(b*b)==(c*c)):
                        return a*b*c

print(specialPythagoreanTriplet())
