#include <stdio.h>

typedef int Bitvector;

typedef struct {
    Bitvector bv;
} BoardA;

typedef struct {
    char cell[6];
} BoardB;

typedef void * Board;

typedef struct {
    Board b;
    int move;
} BoardData;

int main() {
    BoardA b1;
    b1.bv = 0x10;
    printf("[%p] b1: %#x\n", &b1, b1.bv);

    BoardData bd1;
    bd1.b = &b1; // ptr to b1
    bd1.move = 1;
    printf("[%p] bd1: {%#x, %d}\n", &bd1, ((BoardA*)bd1.b)->bv, bd1.move);

    BoardData *bd1P = &bd1;
    printf("bd1 ptr: %p (dereference it: {%#x, %d})\n", bd1P, 
        ((BoardA*)bd1P->b)->bv, bd1P->move);

    Board *bd1BP = bd1P->b;
    printf("bd1's `b` ptr: %p (dereference it: %#x)\n", bd1BP, 
        ((BoardA*)bd1BP)->bv);

    BoardB b2;
    for (int i = 0; i < 5; i++) {
        b2.cell[i] = 'a' + i;
    }
    *(b2.cell+6) = '\0';
    printf("[%p] b2: [0] %c, [2] %c\n", &b2, b2.cell[0], *(b2.cell+2));

    BoardData bd2;
    bd2.b = &b2; // ptr to b2
    bd2.move = 2;
    printf("[%p] bd2: {%s, %d}\n", &bd2, ((BoardB*)bd2.b)->cell, bd2.move);

    BoardData *bd2P = &bd2;
    printf("bd2 ptr: %p (dereference it: {%s, %d})\n", bd2P, 
        ((BoardB*)bd2P->b)->cell, bd2P->move);

    Board *bd2BP = bd2P->b;
    printf("bd2's `b` ptr: %p (dereference it: %s)\n", bd2BP, 
        ((BoardB*)bd2BP)->cell);

    return 0;
}
