#include <iostream>

int main() {
    #if PRINT
    std::cout << "MESSAGE!" << std::endl;
    #endif
}