#include <stdio.h>

typedef int Bitvector;

typedef struct {
    Bitvector bv;
} Board;

typedef struct {
    Board b;
    int move;
} BoardData;

int main() {
    Board b1;
    b1.bv = 0x10;
    Board b2;
    b2.bv = 0x20;

    BoardData bd1;
    bd1.b = b1; // copy of b1
    bd1.move = 1;

    printf("[%p] b1: %#x\n", &b1, b1.bv);
    printf("[%p] b2: %#x\n", &b2, b2.bv);
    printf("[%p] bd1: {%#x, %d}\n", &bd1, bd1.b.bv, bd1.move);

    BoardData *bd1P = &bd1;
    printf("bd1 ptr: %p (dereference it: {%#x, %d})\n", bd1P, bd1P->b.bv, 
        bd1P->move);

    Board *bd1BP = &(bd1P->b);
    printf("bd1's `b` ptr: %p (dereference it: %#x)\n", bd1BP, bd1BP->bv);

    return 0;
}
