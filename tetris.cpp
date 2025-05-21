#include "tetris.hpp"

//piece class

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

    if(pow_of_2(s)){
        m_side = s;
    }
    else{
        throw tetris_exception("piece constructor: side invalid, it must be a pow of 2");
    }


    if(c > 0){
        m_color = c;
    }

    else
        throw tetris_exception("piece constructor: the color must be > 0");

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
    if(m_grid and i < side() and j < side()){
        return m_grid[i][j];
    }
    else
        throw tetris_exception("piece operator(): index of bound");
}

bool& piece::operator()(uint32_t i, uint32_t j) {
    if(m_grid and i < side()  and j < side()){
        return m_grid[i][j];
    }
    else
        throw tetris_exception("piece operator(): index of bound");
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
        throw tetris_exception("piece empty: out of bound");

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
        throw tetris_exception("piece full: out of bound");

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

    for(int k=i;k != 0;--k){
        for(int j=0;j < side();j++){
            m_grid[k][j] = m_grid[k - 1][j];
        }
    }

    for(int j=0;j < side();j++){
        m_grid[0][j] = false;
    }
}

void piece::print_ascii_art(std::ostream& os) const {
    if(m_grid == nullptr) return;

    os << "-";
    for(int i=0;i < side() + 1;++i)
        os << "-";
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

    os << "-";
    for(int i=0;i < side() + 1;++i)
        os << "-";
    os << std::endl;

}

//comparison operators
bool piece::operator==(piece const& rhs) const {
    if(this == &rhs) return true;

    if(side() != rhs.side()) return false;
    if(color() != rhs.color()) return false;

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

bool c_is_int(char c){
    return c >= 48 and c <= 57;
}

void grid_all(piece& p, bool value){
    for(int i=0;i < p.side();++i){
        for(int j=0;j < p.side();++j){
            p(i, j) = value;
        }
    }
}

void GRID(std::istream& is, piece& p){
    if(p.side() < 2) {
        throw tetris_exception("piece GRID: i'm throwing an error");
    }
    char c;
    skip(is);

    int side_2 = p.side() / 2;

    piece pieces[4] = {
        piece(side_2, p.color()),  // tl
        piece(side_2, p.color()),  // tr
        piece(side_2, p.color()),  // bl
        piece(side_2, p.color())   // br
    };

    for(int i=0;i < 4;++i){
        if(is.peek() == '('){
            is >> c;
            skip(is);

            if(is.peek() == ')'){
                is >> c;
                skip(is);

                //pieces[i] all true;
                grid_all(pieces[i], true);

            }
            else{
                GRID(is, pieces[i]);

                if(is.peek() == ')'){
                    is >> c;
                    skip(is);
                }
                else{
                    throw tetris_exception("piece GRID: expeted ) in input");
                }
            }
        }

        else if(is.peek() == '['){
            is >> c;
            skip(is);

            if(is.peek() == ']'){
                is >> c;
                skip(is);
                //pieces[i] all false;

                grid_all(pieces[i], false);
            }
            else{
                throw tetris_exception("piece GRID: expeted ] in input");
            }
        }
        else{
            if(is.peek() == -1)
                break;
            else
                throw tetris_exception("piece GRID: expeted ( or [ in input");
        }
    }

    //piece formed by the subpieces
    for (int i = 0; i < side_2; ++i)
        for (int j = 0; j < side_2; ++j){
            p(i, j) = pieces[0](i, j);
        }
    for (int i = 0; i < side_2; ++i)
        for (int j = 0; j < side_2; ++j)
            p(i, j + side_2) = pieces[1](i, j);

    for (int i = 0; i < side_2; ++i)
        for (int j = 0; j < side_2; ++j)
            p(i + side_2, j) = pieces[2](i, j);

    for (int i = 0; i < side_2; ++i)
        for (int j = 0; j < side_2; ++j)
            p(i + side_2, j + side_2) = pieces[3](i, j);

    skip(is);
}

std::istream& operator>>(std::istream& is, piece& p){
    skip(is);

    int side;
    if(c_is_int(is.peek())){
        is >> side;
    }
    else{
        throw tetris_exception("piece operator>>: expected int");
    }

    skip(is);

    int color;
    if(c_is_int(is.peek())){
        is >> color;
    }
    else{
        throw tetris_exception("piece operator>>: expected int");
    }


    piece p1(side, color);
    p = p1;

    skip(is);

    char c;
    skip(is);

    if(is.peek() == '('){
        is >> c;
        skip(is);

        if(is.peek() == ')'){
            is >> c;
            skip(is);

            //pieces[i] all true;
            grid_all(p, true);
        }
        else{
            GRID(is, p);

            if(is.peek() == ')'){
                is >> c;
                skip(is);
            }
            else{
                throw tetris_exception("piece operator>>: expeted ) in input");
            }
        }
    }

    else if(is.peek() == '['){
        is >> c;
        skip(is);

        if(is.peek() == ']'){
            is >> c;
            skip(is);

            //pieces[i] all false;
            grid_all(p, false);
        }
        else{
            throw tetris_exception("piece operator>>: expeted ] in input");
        }
    }
    else{
        throw tetris_exception("piece operator>>: expeted ( or [ in input");
    }

    if(is.peek() != -1) throw tetris_exception("piece operator>>: expeted no characters at the end");


    return is;
}

//output parser

void piece_output(std::ostream& os, piece const& p){
    int side_2 = p.side() / 2;

    //top left
    if(p.empty(0,0, side_2)){
        os << "([]";
    }
    else if(p.full(0,0, side_2)){
        os << "(()";
    }
    else{
        os << "(";
        piece p_tmp(side_2, p.color());
        for(int i=0;i < side_2;i++){
            for(int j=0;j < side_2;j++){
                p_tmp(i, j) = p(i, j);
            }
        }

        piece_output(os, p_tmp);
    }

    //top right
    if(p.empty(0,side_2, side_2)){
        os << "[]";
    }
    else if(p.full(0,side_2, side_2)){
        os << "()";
    }
    else{
        piece p_tmp(side_2, p.color());

        for(int i=0;i < side_2;++i){
            for(int j=0;j < side_2;++j){
                p_tmp(i, j) = p(i, j + side_2);
            }
        }

        //os << "(";
        piece_output(os, p_tmp);
    }

    //bottom left
    if(p.empty(side_2,0, side_2)){
        os << "[]";
    }
    else if(p.full(side_2,0, side_2)){
        os << "()";
    }
    else{
        piece p_tmp(side_2, p.color());

        for(int i=0;i < side_2;++i){
            for(int j=0;j < side_2;++j){
                p_tmp(i, j) = p(i + side_2, j);
            }
        }

        piece_output(os, p_tmp);
    }

    //bottom right
    if(p.empty(side_2,side_2, side_2)){
        os << "[])";
    }
    else if(p.full(side_2,side_2, side_2)){
        os << "())";
    }
    else{
        piece p_tmp(side_2, p.color());

        for(int i=0;i < side_2;++i){
            for(int j=0;j < side_2;++j){
                p_tmp(i, j) = p(i + side_2, j + side_2);
            }
        }

        piece_output(os, p_tmp);
        os << ")";
    }
}

std::ostream& operator<<(std::ostream& os, piece const& p){
    os << p.side() << " " << p.color() << " ";

    piece_output(os, p);

    return os;
}

//tetris class

//constructors and destructor
tetris::tetris() : m_score(0),m_width(0),m_height(0),m_field(nullptr){}

tetris::tetris(uint32_t w, uint32_t h, uint32_t s) : m_width(w), m_height(h), m_field(nullptr){
    if(m_width == 0){
        throw tetris_exception("tetris constructor: the width must be > 0");
    }

    if(m_height == 0){
        throw tetris_exception("tetris constructor: the height must be > 0");
    }
}

tetris::tetris(tetris const& rhs){
    m_score = rhs.m_score;

    m_width = rhs.m_width;

    m_height = rhs.m_height;

    m_field = nullptr;

    for(node* n = rhs.m_field; n != nullptr; n = n->next){
        m_field = new node{n->tp, n->next};
    }
}

tetris::tetris(tetris&& rhs){
    m_score = rhs.m_score;

    m_width = rhs.m_width;

    m_height = rhs.m_height;

    m_field = rhs.m_field;
    rhs.m_field = nullptr;
}

tetris::~tetris() {
    node* n = m_field;
    while(n != nullptr){
        node* temp = n;
        n = n->next;
        delete temp;
    }
}

//assignment operators
tetris& tetris::operator=(tetris const& rhs){
    if(this != &rhs){
        node* n = m_field;
        node* prev = nullptr;
        while(n != nullptr){
            prev = n;
            n = n->next;

            delete prev;
        }
        if(prev) delete prev;
        m_field = nullptr;

        m_score = rhs.m_score;

        m_width = rhs.m_width;

        m_height = rhs.m_height;

        for(node* n = rhs.m_field; n != nullptr; n = n->next){
            m_field = new node{n->tp, n->next};
        }
    }
    return *this;
}

tetris& tetris::operator=(tetris&& rhs){
    if(this != &rhs){
        node* n = m_field;
        node* prev = nullptr;
        while(n != nullptr){
            prev = n;
            n = n->next;

            delete prev;
        }
        if(prev) delete prev;
        m_field = nullptr;

        m_score = rhs.m_score;

        m_width = rhs.m_width;

        m_height = rhs.m_height;

        m_field = rhs.m_field;
        rhs.m_field = nullptr;
    }
    return *this;
}

//comparison operators
bool tetris::operator==(tetris const& rhs) const{
    if(this == &rhs) return true;

    if(score() != rhs.score()) return false;
    if(width() != rhs.width()) return false;
    if(height() != rhs.height()) return false;

    node* n = rhs.m_field;
    node* m = m_field;

    for (;n != nullptr and m != nullptr;n = n->next, m = m->next) {
        if (n->tp.p != m->tp.p)
            return false;
    }

    if (n != nullptr or m != nullptr)
        return false;

    return true;
}

bool tetris::operator!=(tetris const& rhs) const{
    return !(*this == rhs);
}

//getters
uint32_t tetris::score() const{
    return m_score;
}

uint32_t tetris::width() const{
    return m_width;
}

uint32_t tetris::height() const{
    return m_height;
}

//game operations

void tetris::print_ascii_art(std::ostream& os) const {
    struct cell {
        bool value = false;
        uint8_t color = 0;
    };

    cell** m_grid = new cell*[m_height];
    for (int y = 0; y < m_height; ++y) {
        m_grid[y] = new cell[m_width];
    }

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            m_grid[y][x].value = false;
            m_grid[y][x].color = 0;
        }
    }

    for (node* n = m_field; n != nullptr; n = n->next) {
        for (int dy = 0; dy < n->tp.p.side(); ++dy) {
            for (int dx = 0; dx < n->tp.p.side(); ++dx) {
                if (n->tp.p(dx, dy)) {
                    int gx = n->tp.x + dx;
                    int gy = n->tp.y + dy;

                    if (gx >= 0 && gx < m_width && gy >= 0 && gy < m_height) {
                        m_grid[gy][gx].value = true;
                        m_grid[gy][gx].color = n->tp.p.color();
                    }
                }
            }
        }
    }

    os << "  ";
    for (int x = 0; x < m_width; ++x)
        os << x;
    os << "\n +";
    for (int x = 0; x < m_width; ++x)
        os << "-";
    os << "+\n";

    for (int y = 0; y < m_height; ++y) {
        os << y <<"|";
        for (int x = 0; x < m_width; ++x) {
            if (m_grid[y][x].value)
                os << "\033[48;5;" << int(m_grid[y][x].color) << "m \033[m";
            else
                os << " ";
        }
        os << "|\n";
    }

    os << " +";
    for (int x = 0; x < m_width; ++x)
        os << "-";
    os << "+\n";

    for (int y = 0; y < m_height; ++y)
        delete[] m_grid[y];
    delete[] m_grid;
}

bool tetris::containment(piece const& p, int x, int y) const{
    if(y < 0) throw tetris_exception("tetris containment: the y can't be < 0");

    for(node* n = m_field; n != nullptr; n = n->next){

        int offset_x = n->tp.x;
        int offset_y = n->tp.y;

        for(int i=x; i < m_width; i++){
            for(int j=y; j < m_height; j++){
                if(p(i + x, j + y) and n->tp.p(i + offset_x, j + offset_y)){
                    return false;
                }
            }
        }
    }
    return true;
}

void tetris::add(piece const& p, int x, int y){
    if(containment(p, x, y)){
        tetris_piece tp{p, x, y};

        node* n = new node{tp, m_field};
        m_field = n;
    }
    else{
        throw tetris_exception("tetris add: containment failed");
    }
}

void tetris::insert(piece const& p, int x){
    
}