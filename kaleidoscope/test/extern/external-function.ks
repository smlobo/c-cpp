# Test extern functions

extern sin(x);
extern cos(x);

def testextern(x)
    sin(x)*sin(x) + cos(x)*cos(x);
