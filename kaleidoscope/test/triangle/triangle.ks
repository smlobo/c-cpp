# Test multiple functions, multiple expressions for functions

extern putchard(x);

def drawrow(n)
    for i = 1, i < n, 1 in
        putchard(88)
    putchard(10)

def triangle(n)
    for i = 1, i < n, 1 in
        drawrow(i)
