# Test reassociation of expressions & common sub-expression elimination

def testcse(x)
    (1+2+x)*(x+(1+2));
