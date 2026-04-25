#include "digits.hpp"
#include <cstdlib>

Cell cleanupCell(Cell cell)
{
    return cell;
}


Cell recognizeEmpty(Cell cell)
{
    // STUB: temporary logic for pipeline testing
    cell.value = rand() % 2;
    return cell;
}
