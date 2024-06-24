//
// Created by nuh alsharafi on 5/12/24.
//

#ifndef HOMEWORK5_PLAYER_H
#define HOMEWORK5_PLAYER_H

//include board
#include "Board.h"

class Player
{
private:
    Board & ourBoard;
    char myPlayer;
public:
    Player (Board &, char);
    bool put (int);
    void turn_board();
    bool is_winner();
    char get_symbol() const;
};

#endif //HOMEWORK5_PLAYER_H
