//
// Created by nuh alsharafi on 5/12/24.
//

#ifndef HOMEWORK5_BOARD_H
#define HOMEWORK5_BOARD_H

class Board
{
private:
    char** board;
    int size;           //board has size*size blocks
    int N;              //the required length of the connect sequence to win
    int orientation;    //this will keep track of the board's rotation with values
                        // 0-3 then it wraps around
    void gravity();     //function applies gravity depending on board orientation
public:
    Board(int size, int N);
    ~Board();
    void print_board() const;
    bool put_piece(char piece, int column);
    void turn_board_clockwise();
    char get_game_status();
};


#endif //HOMEWORK5_BOARD_H
