//
// Created by Sheldon Lobo on 2/26/24.
//

#include "utilities.h"

#include <cstdlib>
#include <ctime>
#include <cassert>

class PreMain {
public:
    PreMain() {
        // cout << "PreMain constructor\n";

        // Random seed based on time
        srand(time(nullptr));
    }
};

PreMain premain;

// Random double between -1 and 1
double randomD() {
    double d = (double)rand()/(double)(RAND_MAX/2);
    return d - 1.0;
}

// Random int between from (inclusive) and to (exclusive)
int randomInt(int from, int to) {
    assert(from < to);
    int diff = to - from;
    int r = rand() % diff;
    return r + from;
}
