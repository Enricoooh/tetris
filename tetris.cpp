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
    for (uint32_t i=0; i < side(); ++i){
        m_grid[i] = new bool[side()];
    }

    for(uint32_t i=0;i < m_side;++i){
        for(uint32_t j=0;j < m_side;++j){
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
        for (uint32_t i=0;i < m_side;++i){
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

        for (uint32_t i=0;i < m_side;++i){
            delete[] m_grid[i];
        }
        delete[] m_grid;

        m_grid = nullptr;

        m_grid = new bool*[side()];
        for (uint32_t i=0; i < side(); ++i){
            m_grid[i] = new bool[side()];
        }

        for(uint32_t i=0;i < m_side;++i){
            for(uint32_t j=0;j < m_side;++j){
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

        for (uint32_t i=0;i < m_side;++i) {
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
        throw tetris_exception("piece operator(): index out of bound: " + std::to_string(i) + ", " + std::to_string(j));
}

bool& piece::operator()(uint32_t i, uint32_t j) {
    if(m_grid and i < side()  and j < side()){
        return m_grid[i][j];
    }
    else
        throw tetris_exception("piece operator(): index out of bound: " + std::to_string(i) + ", " + std::to_string(j));
}

//piece operations
bool piece::empty() const {
    if(m_grid == nullptr) return true;

    for(uint32_t i=0;i < side();++i){
        for(uint32_t j=0;j < side();++j){
            if(this->operator()(i, j) == true)
                return false;
        }
    }

    return true;
}

bool piece::full() const {
    if(m_grid == nullptr) return false;

    for(uint32_t i=0;i < side();++i){
        for(uint32_t j=0;j < side();++j){
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

    for(uint32_t m=i;m < i + s;++m){
        for(uint32_t n=j;n < j + s;++n){
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

    for(uint32_t m=i;m < i + s;++m){
        for(uint32_t n=j;n < j + s;++n){
            if(operator()(m, n) == false)
                return false;
        }
    }

    return true;
}

void piece::rotate() {
    bool** m_grid_r = new bool*[side()];

    for (uint32_t i=0; i < side(); ++i){
        m_grid_r[i] = new bool[side()];
    }

    for(uint32_t i=0;i < side();++i){
        for(uint32_t j=0;j < side();++j){
            m_grid_r[j][m_side - i - 1] = m_grid[i][j];
        }
    }

   for (uint32_t i=0;i < m_side;++i){
        delete[] m_grid[i];
    }
    delete[] m_grid;

    m_grid = m_grid_r;
}

void piece::cut_row(uint32_t i) {
    if(m_grid == nullptr or i >= side()) {
        throw tetris_exception("cut_row: invalid input");
    }

    for(uint32_t k=i;k != 0;--k){
        for(uint32_t j=0;j < side();j++){
            m_grid[k][j] = m_grid[k - 1][j];
        }
    }

    for(uint32_t j=0;j < side();j++){
        m_grid[0][j] = false;
    }
}

void piece::print_ascii_art(std::ostream& os) const {
    if(m_grid == nullptr) return;

    os << "-";
    for(uint32_t i=0;i < side() + 1;++i)
        os << "-";
    os << std::endl;

    for(uint32_t i=0;i < side();++i){
        os << "|";
        uint32_t j;
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
    for(uint32_t i=0;i < side() + 1;++i)
        os << "-";
    os << std::endl;

}

//comparison operators
bool piece::operator==(piece const& rhs) const {
    if(this == &rhs) return true;

    if(side() != rhs.side()) return false;
    if(color() != rhs.color()) return false;

    for(uint32_t i=0;i < side();++i){
        for(uint32_t j=0;j < side();++j){
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
    for(uint32_t i=0;i < p.side();++i){
        for(uint32_t j=0;j < p.side();++j){
            p(i, j) = value;
        }
    }
}

void GRID(std::istream& is, piece& p){
    if(p.side() < 2 or !pow_of_2(p.side())) {
        throw tetris_exception("piece GRID: piece in parser too big");
    }

    char c;
    skip(is);

    uint32_t side_2 = p.side() / 2;

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

    try{
        //piece formed by the subpieces
        for (uint32_t i = 0; i < side_2; ++i)
            for (uint32_t j = 0; j < side_2; ++j){
                p(i, j) = pieces[0](i, j);
            }
        for (uint32_t i = 0; i < side_2; ++i)
            for (uint32_t j = 0; j < side_2; ++j)
                p(i, j + side_2) = pieces[1](i, j);

        for (uint32_t i = 0; i < side_2; ++i)
            for (uint32_t j = 0; j < side_2; ++j)
                p(i + side_2, j) = pieces[2](i, j);

        for (uint32_t i = 0; i < side_2; ++i)
            for (uint32_t j = 0; j < side_2; ++j)
                p(i + side_2, j + side_2) = pieces[3](i, j);
    } catch(tetris_exception e){
        throw tetris_exception("piece GRID: error in matrix formation: " + e.what());
    }

    skip(is);
}

std::istream& operator>>(std::istream& is, piece& p){
    skip(is);

    uint32_t side;
    if(c_is_int(is.peek())){
        is >> side;
        if(side > p.side() or side == 0 or !pow_of_2(side))
            throw tetris_exception("piece operator>>: side is too big");
    }
    else{
        throw tetris_exception("piece operator>>: expected int");
    }

    skip(is);

    int color;
    if(c_is_int(is.peek())){
        is >> color;
        if (color == 0)
            throw tetris_exception("piece operator>>: color must be > 0");
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

    return is;
}

//output parser

void piece_output(std::ostream& os, piece const& p){

    if(p.side() == 1){
        if(p.full() == true)
            os << "()";
        else if(p.empty() == true)
            os << "[]";
        else
            throw tetris_exception("piece piece_output: unexpected error");
        return;
    }

    uint32_t side_2 = p.side() / 2;

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
        for(uint32_t i=0;i < side_2;i++){
            for(uint32_t j=0;j < side_2;j++){
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

        for(uint32_t i=0;i < side_2;++i){
            for(uint32_t j=0;j < side_2;++j){
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

        for(uint32_t i=0;i < side_2;++i){
            for(uint32_t j=0;j < side_2;++j){
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

        for(uint32_t i=0;i < side_2;++i){
            for(uint32_t j=0;j < side_2;++j){
                p_tmp(i, j) = p(i + side_2, j + side_2);
            }
        }

        piece_output(os, p_tmp);
        os << ")";
    }
}

std::ostream& operator<<(std::ostream& os, piece const& p){
    if (p.side() == 0 or !pow_of_2(p.side()))
        throw tetris_exception("operator<<: piece has invalid side");

    os << p.side() << " " << p.color() << " ";

    piece_output(os, p);

    return os;
}

//tetris class

//constructors and destructor
tetris::tetris() : m_score(0),m_width(0),m_height(0),m_field(nullptr){}

tetris::tetris(uint32_t w, uint32_t h, uint32_t s) : m_score(s), m_width(w), m_height(h), m_field(nullptr){
    if(m_width == 0)
        throw tetris_exception("tetris constructor: the width must be > 0");

    if(m_height == 0)
        throw tetris_exception("tetris constructor: the height must be > 0");
}

tetris::tetris(tetris const& rhs) : m_score(rhs.m_score), m_width(rhs.m_width), m_height(rhs.m_height), m_field(nullptr){
    if (rhs.m_field == nullptr) return;

    m_field = new node{rhs.m_field->tp,nullptr};
    node* cur = m_field;
    node* src = rhs.m_field->next;

    while(src != nullptr){
        cur->next = new node{src->tp,nullptr};
        cur = cur->next;
        src = src->next;
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
        m_field = nullptr;

        m_score = rhs.m_score;
        m_width = rhs.m_width;
        m_height = rhs.m_height;

        if (rhs.m_field != nullptr){
            m_field = new node{rhs.m_field->tp, nullptr};
            node* cur = m_field;
            node* src = rhs.m_field->next;

            while (src != nullptr){
                cur->next = new node{src->tp, nullptr};
                cur = cur->next;
                src = src->next;
            }
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
        if(n->tp.p != m->tp.p) return false;
    }

    if (n != nullptr or m != nullptr)
        return false;

    return true;
}

bool tetris::operator!=(tetris const& rhs) const{ return !(*this == rhs); }

//getters
uint32_t tetris::score() const{ return m_score; }

uint32_t tetris::width() const{ return m_width; }

uint32_t tetris::height() const{ return m_height; }

//game operations

void tetris::print_ascii_art(std::ostream& os) const {
    struct cell {
        bool value = false;
        uint8_t color = 0;
    };

    // Griglia di gioco temporanea
    cell** m_grid = new cell*[m_height];
    for (uint32_t y = 0; y < m_height; ++y)
        m_grid[y] = new cell[m_width];

    // Riempimento della griglia da tutti i pezzi presenti
    for (node* n = m_field; n != nullptr; n = n->next) {
        uint32_t s = n->tp.p.side();
        for (uint32_t dy = 0; dy < s; ++dy) {
            for (uint32_t dx = 0; dx < s; ++dx) {
                if (n->tp.p(dy, dx)) {
                    uint32_t gx = n->tp.x + dx;
                    uint32_t gy = n->tp.y + dy;
                    if (gx < m_width && gy < m_height) {
                        m_grid[gy][gx].value = true;
                        m_grid[gy][gx].color = n->tp.p.color();
                    }
                }
            }
        }
    }

    // Stampa cornice superiore
    os << "-";
    for (uint32_t i = 0; i < m_width + 1; ++i)
        os << "-";
    os << "\n";

    // Stampa contenuto riga per riga
    for (uint32_t y = 0; y < m_height; ++y) {
        os << "|";
        for (uint32_t x = 0; x < m_width; ++x) {
            if (m_grid[y][x].value)
                os << "\033[48;5;" << int(m_grid[y][x].color) << "m" << ' ' << "\033[m";
            else
                os << ' ';
        }
        os << "|\n";
    }

    // Stampa cornice inferiore
    os << "-";
    for (uint32_t i = 0; i < m_width + 1; ++i)
        os << "-";
    os << "\n";

    // Cleanup
    for (uint32_t y = 0; y < m_height; ++y)
        delete[] m_grid[y];
    delete[] m_grid;
}

/*bool tetris::containment(piece const& p, int x, int y) const{
    if(y < 0) return false;

    for (int j = y; j < m_height and y-j < p.side(); ++j){
        for (int i = x; i < m_width and x-i < p.side(); ++i){
            std::cout << i << ", " << j << std::endl;
            if(p(i - x, j - y)){
                if(i >= 0 and j >= 0 and i < m_width and j < m_height){

                    for (const_iterator it = begin(); it != end(); ++it){
                        for (int ty = it->y; ty < m_height and it->y-ty < it->p.side(); ++ty){
                            for (int tx = it->x; tx < m_width and it->x - tx < it->p.side(); ++tx){
                                //std::cout << tx << ", " << ty;
                                if(it->p(tx - it->x, ty - it->y)){
                                    if(it->x + tx == i and it->y + ty == j) return false;
                                }

                            }
                        }
                    }

                }
                else return false;
            }
        }
    }

    return true;
}*/

bool tetris::containment(const piece& p, int x, int y) const {
    if (y < 0) return false; // controlla che non si vada sopra o a sinistra

    for (uint32_t dy = 0; dy < p.side(); ++dy) {
        for (uint32_t dx = 0; dx < p.side(); ++dx) {
            if (p(dx, dy)) {
                int grid_x = x + dx;
                int grid_y = y + dy;

                // fuori dai limiti della griglia
                if (grid_y < 0 || grid_y >= static_cast<int>(m_height)) {
                    return false;
                }

                // controllo sovrapposizioni con pezzi già presenti
                for (const_iterator it = begin(); it != end(); ++it) {
                    for (uint32_t pd_y = 0; pd_y < it->p.side(); ++pd_y) {
                        for (uint32_t pd_x = 0; pd_x < it->p.side(); ++pd_x) {
                            if (it->p(pd_x, pd_y)) {
                                int existing_x = it->x + pd_x;
                                int existing_y = it->y + pd_y;
                                if (existing_x == grid_x && existing_y == grid_y) {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}

void tetris::add(piece const& p, int x, int y){
    if (!containment(p, x, y)) {
        throw tetris_exception("tetris add: containment failed");
    }

    tetris_piece tp{p, x, y};
    node* n = new node{tp, m_field};
    m_field = n;
}

void tetris::insert(piece const& p, int x){
    
}

//iterator
tetris::iterator::iterator(node* ptr) : m_ptr(ptr) {}

tetris::iterator::reference tetris::iterator::operator*(){
    return m_ptr->tp;
}

tetris::iterator::pointer tetris::iterator::operator->(){
    return &m_ptr->tp;
}

tetris::iterator& tetris::iterator::operator++(){
    if (m_ptr) m_ptr = m_ptr->next;
    return *this;
}

tetris::iterator tetris::iterator::operator++(int){
    iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool tetris::iterator::operator==(const iterator& rhs) const{
    return m_ptr == rhs.m_ptr;
}

bool tetris::iterator::operator!=(const iterator& rhs) const{
    return !(*this == rhs);
}

//const_iterator
tetris::const_iterator::const_iterator(node const* ptr) : m_ptr(ptr) {}

tetris::const_iterator::reference tetris::const_iterator::operator*() const{
    return m_ptr->tp;
}

tetris::const_iterator::pointer tetris::const_iterator::operator->() const{
    return &m_ptr->tp;
}

tetris::const_iterator& tetris::const_iterator::operator++(){
    if (m_ptr) m_ptr = m_ptr->next;
    return *this;
}

tetris::const_iterator tetris::const_iterator::operator++(int){
    const_iterator tmp = *this;
    ++(*this);
    return tmp;
}

bool tetris::const_iterator::operator==(const_iterator const& rhs) const{
    return m_ptr == rhs.m_ptr;
}

bool tetris::const_iterator::operator!=(const_iterator const& rhs) const{
    return !(*this == rhs);
}

//iterator functions
tetris::iterator tetris::begin(){
    return iterator(m_field);
}

tetris::iterator tetris::end(){
    return iterator(nullptr);
}

tetris::const_iterator tetris::begin() const{
    return const_iterator(m_field);
}

tetris::const_iterator tetris::end() const{
    return const_iterator(nullptr);
}

//streams
std::ostream& operator<<(std::ostream& os, tetris const& t){
    os << t.score() << " " << t.width() << " " << t.height() << " ";

    for (auto i = t.begin(); i != t.end(); ++i){
        os << i->p << "\n";
    }

    return os;
}

std::istream& operator>>(std::istream& is, tetris& t){
    skip(is);

    int score;
    if(c_is_int(is.peek())){
        is >> score;
    }
    else{
        throw tetris_exception("tetris operator>>: expected int");
    }

    skip(is);

    int width;
    if(c_is_int(is.peek())){
        is >> width;
        if(width == 0) throw tetris_exception("tetris operator>>: expected > 0");
    }
    else{
        throw tetris_exception("tetris operator>>: expected int");
    }

    skip(is);

    int height;
    if(c_is_int(is.peek())){
        is >> height;
        if(height == 0) throw tetris_exception("tetris operator>>: expected > 0");
    }
    else{
        throw tetris_exception("tetris operator>>: expected int");
    }

    tetris t1(width, height, score);
    t = t1;

    skip(is);

    while(!c_is_int(is.peek())){
        piece p;
        is >> p;

        int x, y;
        if(c_is_int(is.peek())){
            is >> x;
        }
        else{
            throw tetris_exception("tetris operator>>: expected int");
        }

        if(c_is_int(is.peek())){
            is >> y;
            if(y < 0) throw tetris_exception("tetris operator>>: expected y >= 0");
        }
        else{
            throw tetris_exception("tetris operator>>: expected int");
        }

        t.add(p, x, y);
    }

    return is;
}