#include <iostream>
#include <fstream>
#include "piece.cpp"

int main() {
    try{
        std::ifstream file("input.txt");

        piece p(8, 20);

        file >> p;
        //4 75 (([]()[]())(()[]()[])([]()()())(()[]()()))

        p.print_ascii_art(std::cout);

        std::cout << p;
    }catch(tetris_exception e){
        std::cout << e.what();
    }

}