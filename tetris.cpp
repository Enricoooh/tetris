#include "tetris.hpp"

/* Your implementation goes here! */
piece::piece(){
    m_side = 0;
    m_color = 0;
    m_grid = nullptr;
}

bool pow_of_2(int n){
    if (n < 1)
        return false;
    if (n == 1)
        return true;
    if (n % 2 != 0)
        return false;
    else
        return pow_of_2(n/2);
}

piece::piece(uint32_t s, uint8_t c){
    if(pow_of_2(s))
        m_side = s;
    else
        throw tetris_exception("the side must be a pow of 2");

    if(c > 0)
        m_color = c;
    else
        throw tetris_exception("the color must be > 0");


}