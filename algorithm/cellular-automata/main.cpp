#include "cellular_automata.h"

int main(void) {
    cellular_automata grid(100,100);
    grid.initalize(0.5f);
    grid.simulate(1);
    grid.print();
    return 0;
}