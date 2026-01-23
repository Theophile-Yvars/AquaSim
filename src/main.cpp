#include <iostream>
#include "ocean.hpp"

int main() {
    std::cout << "START\n";
    Ocean ocean;
    std::cout << "AFTER CONSTRUCTOR\n";
    std::cout << "AFTER INIT\n";
    ocean.runSimulation(200);
    return EXIT_SUCCESS;
}