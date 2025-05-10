#include <iostream>
void test(std::istream& is) {
    int a = 0;
    std::string j;
    if(is.peek() >= 48 and is.peek() < 58){
        is >> a;
        std::cout << "é INT";
    }
    else{
        std::cout << "NON é INT";
    }
}

int main(){
    test(std::cout);
}