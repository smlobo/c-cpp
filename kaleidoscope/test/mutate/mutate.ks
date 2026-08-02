# Test mutating a local variable

# Function to print a double.
extern printd(x);

# Define ':' for sequencing: as a low-precedence operator that ignores operands
# and just returns the RHS.
def binary: 1 (x y) y;

def testmutate(x)
    printd(x) :
    x = 4 :
    printd(x);
