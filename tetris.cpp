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
        throw tetris_exception("constructor: side invalid, it must be a pow of 2");
    }


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
        throw tetris_exception("i'm throwing an error");
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
                    throw tetris_exception("GRID: expeted ) in input");
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
                throw tetris_exception("GRID: expeted ] in input");
            }
        }
        else{
            if(is.peek() == -1)
                break;
            else
                throw tetris_exception("GRID: expeted ( or [ in input");
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
        throw tetris_exception("operator>>: expected int");
    }

    skip(is);

    int color;
    if(c_is_int(is.peek())){
        is >> color;
    }
    else{
        throw tetris_exception("operator>>: expected int");
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
                throw tetris_exception("operator>>: expeted ) in input");
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
            throw tetris_exception("operator>>: expeted ] in input");
        }
    }
    else{
        throw tetris_exception("operator>>: expeted ( or [ in input");
    }

    if(is.peek() != -1) throw tetris_exception("operator>>: expeted no characters at the end");


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