#include "digits.hpp"
#include <cstdlib>

void cleanupCell(Cell& cell) {
    return;
}


void recognizeEmpty(Cell& cell) {
    // STUB: temporary logic for pipeline testing
    if (rand() % 2 == 0) {
        cell.value = EMPTY;
    } else {
        cell.value = UNKNOWN;
    }
    return;
}
