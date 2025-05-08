#include <iostream>
#include <cassert>
#include <cstdint>

struct tetris_exception {
    tetris_exception(std::string const& str) : m_str(str) {}
    std::string what() const { return m_str; }

private:
    std::string m_str;
};

struct piece {
    piece();
    piece(uint32_t s, uint8_t c);
    piece(piece const& rhs);
    piece(piece&& rhs);
    ~piece();

    piece& operator=(piece const& rhs);
    piece& operator=(piece&& rhs);

    bool operator==(piece const& rhs) const;
    bool operator!=(piece const& rhs) const;

    bool& operator()(uint32_t i, uint32_t j);
    bool operator()(uint32_t i, uint32_t j) const;

    bool empty(uint32_t i, uint32_t j, uint32_t s) const;
    bool full(uint32_t i, uint32_t j, uint32_t s) const;
    bool empty() const;
    bool full() const;

    void rotate();
    void cut_row(uint32_t i);
    void print_ascii_art(std::ostream& os) const;

    uint32_t side() const;
    int color() const;

private:
    uint32_t m_side;
    uint8_t m_color;
    bool** m_grid;
};

/*std::istream& operator>>(std::istream& is, piece& p);
std::ostream& operator<<(std::ostream& os, piece const& p);*/

//constructors and destructor
piece::piece() {
    m_side = 0;
    m_color = 0;
    m_grid = nullptr;
}

bool pow_of_2(int n) {
    if (n < 1)
        return false;
    if (n == 1)
        return true;
    if (n % 2 != 0)
        return false;
    else
        return pow_of_2(n/2);
}

piece::piece(uint32_t s, uint8_t c) {
    if(pow_of_2(s))
        m_side = s;
    else
        throw tetris_exception("constructor: the side must be a pow of 2");

    if(c > 0)
        m_color = c;
    else
        throw tetris_exception("constructor: the color must be > 0");

    m_grid = new bool*[m_side];

    for (uint32_t i = 0; i < m_side; ++i) {
        m_grid[i] = new bool[m_side];
        for (uint32_t j = 0; j < m_side; ++j) {
            m_grid[i][j] = false;
        }
    }
}

piece::piece(piece const& rhs) {
    m_side = rhs.side();

    m_color = rhs.color();

    m_grid = new bool*[side()];
    for (int i=0; i < side(); ++i){
        m_grid[i] = new bool[side()];
    }

    for(int i=0;i < m_side;++i){
        for(int j=0;j < m_side;++j){
            m_grid[i][j] = rhs.m_grid[i][j];
        }
    }
}

piece::piece(piece&& rhs) {
    m_side = rhs.side();

    m_color = rhs.color();

    m_grid = rhs.m_grid;
    rhs.m_grid = nullptr;
}

piece::~piece() {
    if(m_grid){
        for (int i=0;i < m_side;++i){
            delete[] m_grid[i];
        }
        delete[] m_grid;
    }
}

//assignment operators
piece& piece::operator=(piece const& rhs) {
    if(this != &rhs){
        m_side = rhs.side();

        m_color = rhs.color();

        for (int i=0;i < m_side;++i){
            delete[] m_grid[i];
        }
        delete[] m_grid;

        m_grid = nullptr;

        m_grid = new bool*[side()];
        for (int i=0; i < side(); ++i){
            m_grid[i] = new bool[side()];
        }

        for(int i=0;i < m_side;++i){
            for(int j=0;j < m_side;++j){
                m_grid[i][j] = rhs.m_grid[i][j];
            }
        }
    }

    return *this;
}

piece& piece::operator=(piece&& rhs) {
    if(this != &rhs){
        m_side = rhs.side();

        m_color = rhs.color();

        for (int i=0;i < m_side;++i) {
            delete[] m_grid[i];
        }
        delete[] m_grid;

        m_grid = rhs.m_grid;
        rhs.m_grid = nullptr;
    }

    return *this;
}

//access and mutators
uint32_t piece::side() const {
    return m_side;
}

int piece::color() const {
    return m_color;
}

bool piece::operator()(uint32_t i, uint32_t j) const {
    if(m_grid and i < side()  and j < side()){
        return m_grid[i][j];
    }
    else
        throw tetris_exception("operator(): out of bound");
}

bool& piece::operator()(uint32_t i, uint32_t j) {
    if(m_grid and i < side()  and j < side()){
        return m_grid[i][j];
    }
    else
        throw tetris_exception("operator(): out of bound");
}

//piece operations
bool piece::empty() const {
    if(m_grid == nullptr) return true;

    for(int i=0;i < side();++i){
        for(int j=0;j < side();++j){
            if(this->operator()(i, j) == true)
                return false;
        }
    }

    return true;
}

bool piece::full() const {
    if(m_grid == nullptr) return false;

    for(int i=0;i < side();++i){
        for(int j=0;j < side();++j){
            if(operator()(i, j) == false)
                return false;
        }
    }

    return true;
}

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const {
    if((i + s) > side() or (j + s) > side())
        throw tetris_exception("empty: out of bound");

    if(m_grid == nullptr) return true;

    for(int m=i;m < i + s;++m){
        for(int n=j;n < j + s;++n){
            if(this->operator()(m, n) == true)
                return false;
        }
    }

    return true;
}

bool piece::full(uint32_t i, uint32_t j, uint32_t s) const {
    if(i + s > side() or j + s > side())
        throw tetris_exception("full: out of bound");

    if(m_grid == nullptr) return false;

    for(int m=i;m < i + s;++m){
        for(int n=j;n < j + s;++n){
            if(operator()(m, n) == false)
                return false;
        }
    }

    return true;
}

void piece::rotate() {
    bool** m_grid_r = new bool*[side()];

    for (int i=0; i < side(); ++i){
        m_grid_r[i] = new bool[side()];
    }

    for(int i=0;i < side();++i){
        for(int j=0;j < side();++j){
            m_grid_r[j][m_side - i - 1] = m_grid[i][j];
        }
    }

   for (int i=0;i < m_side;++i){
        delete[] m_grid[i];
    }
    delete[] m_grid;

    m_grid = m_grid_r;
}

void piece::cut_row(uint32_t i) {
    if(m_grid == nullptr) return;

    for(int j=0;j < side();j++){
        m_grid[i][j] = false;
    }

    for(int j=0;j < side();j++){
        m_grid[0][j] = false;
    }

    for(int k=2;k < side();++k){
        for(int j=0;j < side();++j){
            m_grid[k][j] = m_grid[k - 1][j];
        }
    }
}

void piece::print_ascii_art(std::ostream& os) const {
    if(m_grid == nullptr) return;

    os << " ";
    for(int i=0;i < side();++i)
        os << "_";
    os << std::endl;

    for(int i=0;i < side();++i){
        os << "|";
        int j;
        for(j=0;j < side();++j){
            if (m_grid[i][j])
                os << "\033[48;5;" << int(m_color) << "m" << ' ' << "\033[m";
            else
                os << ' ';

        }
        os << "|";

            os << std::endl;
    }

    os << " ";
    for(int i=0;i < side();++i)
        os << "-";
    os << std::endl;

}

//comparison operators
bool piece::operator==(piece const& rhs) const {
    if(this == &rhs) return true;

    for(int i=0;i < side();++i){
        for(int j=0;j < side();++j){
            if(m_grid[i][j] != rhs.m_grid[i][j]) return false;
        }
    }

    return true;
}

bool piece::operator!=(piece const& rhs) const {
    return !(*this == rhs);
}

//parser

void skip(std::istream& is){
    char c = 0;
    is >> c;
    is.putback(c);
}

/*
std::istream& piece::operator>>(std::istream& is, piece& p){
    skip(is);

    //doesn't control if it's int
    is >> p.m_side;

    skip(is);

    //doesn't control if it's int
    is >> p.m_color;

    skip(is);

    p.m_grid = new bool*[side];
    input_to_grid(is, m_side);

    skip(is);
}

void input_to_grid(std::istream& is, bool** grid){
    if(is == nullptr or grid == nullptr) return
}*/
