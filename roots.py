def polyVal (coef, x):
    poly = 0
    for i in range (len (coef)):
        poly += coef [i] * (x ** i)
    return poly

def derivPoly (coef):
    newCoef = []
    for i in range (len (coef) - 1):
        newCoef.append (coef [i + 1] * (i + 1))
    return newCoef

def rtFinder (coef, a, b, m, r, t):
    polyA, polyB = polyVal (coef, a), polyVal (coef, b)
    if polyA == 0:
        return a
    elif polyB == 0:
        return b
    elif polyA * polyB < 0:
        c = (a + b) / 2
        if abs (a - c) < m:
            return c
        else:
            polyC = polyVal (coef, c)
            if polyA * polyC < 0:
                return rtFinder (coef, a, c, m, r, t)
            else:
                return rtFinder (coef, b, c, m, r, t)
    else:
        newCoef = derivPoly (coef)
        c = rtFinder (newCoef, a, b, m, r, t)
        if c != None:
            if abs (polyVal (coef, c)) < t:
                return c

def multiRtFinder (coef, a, b, m, r, t, roots):  
    for i in range (int (a / r), int (b / r)):
        possibleRt = rtFinder (coef, i * r, (i + 1) * r, m, r, t)
        if possibleRt and possibleRt not in roots:
            roots.append (possibleRt)        
    for i in range (len (roots)):
        roots [i] = round (roots [i], 5)    
    return roots

polynomial = input ('Enter the polynomial coefficients: ').split ()
a, b = input ('Enter the interval: ').split ()
a, b = float (a), float (b)
for i in range (len (polynomial)):
    polynomial [i] = float (polynomial [i])
m = 10 ** -6
r = 10 ** -2
t = 10 ** -3
roots = []
solution = multiRtFinder (polynomial, a, b, m, r, t, roots)
if len (solution) != 0:
    for i in solution:
        print (f'Root found at {i}')
else:
    print ('No roots are found!')
