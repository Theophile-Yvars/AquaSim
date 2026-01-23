#include <iostream>
#include "ocean.hpp"

int main() {
    std::cout << "START\n";
    Ocean ocean;
    std::cout << "AFTER CONSTRUCTOR\n";
    ocean.init();
    std::cout << "AFTER INIT\n";
    ocean.runSimulation(21);
    return EXIT_SUCCESS;
}
