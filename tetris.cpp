#include "tetris.hpp"

//implementation piece class

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

    m_grid = nullptr;
}

piece::piece(piece const& rhs) {
    m_side = rhs.side();

    m_color = rhs.color();

    m_grid = new bool*[side()];

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
}

piece::~piece() {
    for (int i=0;i < m_side;++i){
        delete[] m_grid[i];
    }
    delete[] m_grid;
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

        for(int i=0;i < m_side;++i){
            for(int j=0;j < m_side;++j){
                m_grid[i][j] = rhs.m_grid[i][j];
            }
        }
    }
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
    }
}

//access and mutators
uint32_t piece::side() const {
    return m_side;
}

int piece::color() const {
    return m_color;
}

bool piece::operator()(uint32_t i, uint32_t j) const {
    return operator(i, j);
}

bool& piece::operator()(uint32_t i, uint32_t j) {
    if(i < side() and j < side())
        return m_grid[i][j];
    else
        throw tetris_exception("operator(): out of bound");
}

//piece operations
bool piece::empty() const {
    if(m_grid == nullptr) return true;

    for(int i=0;i < side();++i){
        for(int j=0;j < side();++j){
            if(*this(i, j) == true)
                return false;
        }
    }

    return true;
}

bool piece::full() const {
    if(m_grid == nullptr) return false;

    for(int i=0;i < side();++i){
        for(int j=0;j < side();++j){
            if(*this(i, j) == false)
                return false;
        }
    }

    return true;
}

bool piece::empty(uint32_t i, uint32_t j, uint32_t s) const {
    if(i + s > side() or j + s > side())
        throw tetris_exception("empty: out of bound");

    if(m_grid == nullptr) return true;

    for(int m=i;m < i + s;++m){
        for(int n=j;n < j + s;++n){
            if(*this(m, n) == true)
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
            if(*this(m, n) == false)
                return false;
        }
    }

    return true;
}

void piece::rotate() {
    assert(m_side-i-1 >= 0 and m_side-i-1 < side());

    bool** m_grid_r = new bool*[size()];

    for(int i=0;i < side();++i){
        for(int j=0;j < side();++j){
            m_grid_r[i][j] = m_grid[j][m_side-i-1];
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

    delete[] m_grid[i];

    for(int j=0;j < i;++j){
        m_grid[j] = m_grid[j - 1];
    }
}

void piece::print_ascii_art(std::ostream& os) const {
    if (m_grid[i][j]) {
        os << "\033[48;5;" << int(m_color) << "m" << ' ' << "\033[m";
    } else {
        os << ' ';
    }
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
}