#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "predicate.h"

void compareInts(int a, int b) {
    std::strong_ordering result = a <=> b;
    //switch (result) {
    //case std::strong_ordering::less:
    //    std::cout << a << " is less than " << b << std::endl;
    //    break;
    //case std::strong_ordering::equal:
    //    std::cout << a << " is equal to " << b << std::endl;
    //    break;
    //case std::strong_ordering::greater:
    //    std::cout << a << " is greater than " << b << std::endl;
    //    break;
    //}
}

int main(void) noexcept {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    int x = 5;
    int y = 10;

    compareInts(x, y);  // x < y
    compareInts(y, x);  // y > x
    compareInts(x, x);  // x == x

    return 0;
}