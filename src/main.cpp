#include <iostream>
#include "ocean.hpp"

using namespace std;

int main(int argc, char** argv){
    cout << "Hello Wolrd!"<<endl;

    Ocean ocean = Ocean();
    ocean.init();

    return EXIT_SUCCESS;
}