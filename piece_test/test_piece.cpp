#include <iostream>
#include <sstream>
#include <fstream>
#include "../tetris.cpp"

int main() {
    try{
        int dim = 128;
        piece p(dim, 26);

        //std::cout << "em: " << p.empty() << std::endl;

    /*    for(int i=0;i < dim;++i){
            for(int j=0;j < dim;++j){
                p(i, j) = true;
            }
        }*/

        //p.print_ascii_art(std::cout);

        //std::cout << "fu: " << p.full() << std::endl;

        /*for(int i=0;i < 8;++i){
            for(int j=0;j < 8;++j){
                p(i, j) = false;
            }
        }

        try{
            p(0,1) = true;
            p(1,0) = true;
            p(1,1) = true;
        } catch(tetris_exception e){
            std::cout << e.what() << std::endl;
        }*/

        //p.print_ascii_art(std::cout);

    /*    p.rotate();
        p.print_ascii_art(std::cout);

        p.rotate();
        p.print_ascii_art(std::cout);

        p.rotate();
        p.print_ascii_art(std::cout);*/

        /*piece p1 = std::move(p);
        p1.print_ascii_art(std::cout);*/

        piece p2(8, 20);

        p2(0, 0) = false;

        /*try{
            p2(0,0) = true;
        } catch(tetris_exception e){
            std::cout << e.what() << std::endl;
        }

        p2.print_ascii_art(std::cout);*/

        //p = std::move(p2);

        //p.print_ascii_art(std::cout);

        std::ifstream inputFile("input.txt");

        inputFile >> p;
        std::cout << "aaaaa";
        p.print_ascii_art(std::cout);
        std::cout << "bbbbbb";
        p.cut_row(16); p.print_ascii_art(std::cout);
        //p.cut_row(1); p.print_ascii_art(std::cout);
        //p.cut_row(2);
        //p(0, 0) = true;
        //p.print_ascii_art(std::cout);

    /*    std::cout << "empty: " << p.empty(3,2,1);
        std::cout << "full: " << p.full(3,2,1);*/

        /*if(p == p2) std::cout << true;
        else std::cout << false;*/
    } catch(tetris_exception e){
        std::cout << e.what();
    }

}