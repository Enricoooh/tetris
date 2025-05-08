#include <iostream>
#include "piece.cpp"

int main() {
    std::cout << "test" << std::endl;

    piece p(8, 20);

    p(3, 4) = true;
    p(4, 3) = true;
    p(4, 4) = true;
    p(4, 5) = true;

    p.print_ascii_art(std::cout);
}