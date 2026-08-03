# Iterative style fibonacci

# Low precedence sequencing
def binary: 1 (x y) y;

def fibi(x)
    var a = 0, b = 1, c in
    (for i = 2, i < x in
        c = a + b :
        a = b :
        b = c) :
    a + b
