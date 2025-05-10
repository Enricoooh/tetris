//parser

void skip(std::istream& is){
    char c = 0;
    is >> c;
    is.putback(c);
}

bool c_is_int(char c){
    return c >= 48 and c <= 57;
}

//VERSION 1

/*std::istream& piece::operator>>(std::istream& is, piece& p){
    skip(is);

    if(c_is_int(n)){
        is >> p.m_side;
    }
    else{
        throw tetris_exception("operator>>: expected int");
    }

    skip(is);

    if(c_is_int(n)){
        is >> p.m_color;
    }
    else{
        throw tetris_exception("operator>>: expected int");
    }

    skip(is);

    p.m_grid = new bool*[side()];
    for (int i=0; i < side(); ++i){
        p.m_grid[i] = new bool[side()];
    }

    p.m_grid = input_to_grid(is, m_side);
}

bool** input_to_grid(std::istream& is, uint32_t s){
    bool** grid = new bool*[s];
    for (int i=0; i < s; ++i){
        grid[i] = new bool[s];
    }

    *//*    bool** tl = new bool*[s/2];
    for (int i=0; i < s/2; ++i){
        grid[i] = new bool[s/2];
    }

    bool** tr = new bool*[s/2];
    for (int i=0; i < s/2; ++i){
        grid[i] = new bool[s/2];
    }*//*

    char c = 0;
    if(is.peek() == '('){
        is >> c;

        skip(is);

        if(is.peek != ')'){
            skip();
            return input_to_grid
        }

        if(is.peek == ')'){
            is >> c;

            bool** mat = new bool*[1];
            mat[0] = new bool[1];

            *mat[0] = true;
            return mat;
        }
        else{
            return input_to_grid(is, s/2);
        }
    }
    if(is.peek() == '['){
        is >> c;
        skip(is);

        if(is.peek() = ']'){
            is >> c;
            bool** mat = new bool*[1];
            mat[0] = new bool[1];

            *mat[0] = false;
            return mat ;
        }
        else{
            throw tetris_exception("input_to_grid: expected ]");
        }
    }
}

bool** input_to_grid(std::istream& is, uint32_t s){

    if(is.peek() = ')'){
        return
    }

    for(int i=0;i < 4;++i){

    }
}*/

//VERSION 2

/*
void GRID(istream& is, bool** m_grid, int i, int j){
    skip();
    char c;
    if(is.peek() == '['){
        is >> c;
        Q(is, m_grid, i, j);
    }
    else if(is.peek() == '('){
        is >> c;
        T(is, m_grid, i, j);
    }
    else if(is.eof(){
        return;
    }
    else{
        throw tetris_exception("not a GRID value");
    }

}

void Q(istream& is, bool** m_grid, int i, int j){
    skip();
    if(is.peek() == ']'){
        //add value to m_grid
        GRID(is, m_grid, i, j);
    }
    else{
        throw tetris_exception("expected ]");
    }
}

void T(istream& is, bool** m_grid, int i, int j){
    skip();
    if(is.peek() == ')'){
        //add value to m_grid
        GRID(is, m_grid, i, j);
    }

    skip();
    GRID(is, m_grid);
    skip();

    if(is.peek() == ')'){
        is >> c;
        skip();
    }
    else{
        throw tetris_exception("expected )");
    }
}

std::istream& piece::operator>>(std::istream& is, piece& p){
    skip(is);

    if(c_is_int(n)){
        is >> p.m_side;
    }
    else{
        throw tetris_exception("operator>>: expected int");
    }

    skip(is);

    if(c_is_int(n)){
        is >> p.m_color;
    }
    else{
        throw tetris_exception("operator>>: expected int");
    }

    skip(is);

    if(p.m_grid != nullptr){
        //delete the old m_grid
    }

    p.m_grid = new bool*[side()];
    for (int i=0; i < side(); ++i){
        p.m_grid[i] = new bool[side()];
    }

    GRID(is, p.m_grid, 0, 0);
}*/

void grid_all(piece p, bool value, int side){
    for(int i=0;i < side;++i){
        for(int j=0;j < side;++j){
            p(i, j) = value;
        }
    }
}

void GRID(std::istream& is, piece& p, int side){
    std::cout << "entro grid" << std::endl;
    piece pieces[4];
    //allocazione grids
    int side_2 = side / 2;

/*    for(int i = 0; i < 4; ++i){
        grids[i] = new bool*[side_2];
        for(int j = 0; j < side_2; ++j){
            grids[i][j] = new bool[side_2];
            for(int k = 0; k < side_2; ++k){
                grids[i][j][k] = false;
            }
        }
    }*/

    char c;
    skip(is);

    //formazione delle subgrids
    for(int i=0;i < 4;++i){
        std::cout << "i: " << i << std::endl;
        if(is.peek() == '('){
            is >> c;
            skip(is);

            if(is.peek() == ')'){
                is >> c;
                skip(is);

                //grids[i] all false;
                std::cout << "all false";
                grid_all(pieces[i], false, side_2);

            }
            else{
                GRID(is, pieces[i], side/2);

                if(is.peek() == ')'){
                    is >> c;
                    skip(is);
                }
                else{
                    throw tetris_exception("expeted ) in input");
                }
            }
        }

        else if(is.peek() == '['){
            std::cout << "entro [" << std::endl;
            is >> c; std::cout << "mangio" << c << std::endl;
            skip(is);

            if(is.peek() == ']'){
                std::cout << "entro ]" << std::endl;
                is >> c; std::cout << "mangio" << c << std::endl;
                skip(is); std::cout << "skip" << c << std::endl;
                //grids[i] all true;
                std::cout << "all true" << std::endl;
                grid_all(pieces[i], true, side_2);
            }
            else{
                throw tetris_exception("expeted ] in input");
            }
        }
        else{
            if(is.eof()) {return;}
            else
                throw tetris_exception("expeted ( or [ in input");
        }
    }

    //m_grid formata dalle subgrids
    for (int i = 0; i < side_2; ++i)
        for (int j = 0; j < side_2; ++j)
            p(i, j) = pieces[0](i, j);

    for (int i = 0; i < side_2; ++i)
        for (int j = 0; j < side_2; ++j)
            p(i, j + side_2) = pieces[1](i, j);

    for (int i = 0; i < side_2; ++i)
        for (int j = 0; j < side_2; ++j)
            p(i + side_2, j) = pieces[2](i, j);

    for (int i = 0; i < side_2; ++i)
        for (int j = 0; j < side_2; ++j)
            p(i + side_2, j + side_2) = pieces[3](i, j);

/*    //deallocazione grids
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < side_2; ++j) {
            delete[] grids[i][j];
        }
        delete[] grids[i];
    }
    delete[] grids;*/

    skip(is);
    std::cout << "esco grid" << std::endl;
}

std::istream& operator>>(std::istream& is, piece& p){
    std::cout << "entro operatore" << std::endl;
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

/*    if(p.m_grid != nullptr){
        for (int i=0;i < m_side;++i){
            delete[] p.m_grid[i];
        }
        delete[] p.m_grid;
    }

    p.m_grid = new bool*[side()];
    for (int i=0; i < side(); ++i){
        p.m_grid[i] = new bool[side()];
    }*/
    std::cout << "esco operatore: "<< is.peek() << std::endl;
    GRID(is, p, side);

    return is;
}