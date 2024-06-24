//
// Created by nuh alsharafi on 5/12/24.
//

#include "Player.h"


//default constructor
Player::Player (Board & game, char player)
: ourBoard(game), myPlayer(player)
{ }

//put
bool Player::put(int column)
{
    /* Function takes the column a player wants to place a piece in, and passes
     * it to put_piece from Board class*/
    return ourBoard.put_piece(myPlayer, column);
}


//turn_board
void Player::turn_board()
{
    /* Function turns the board using the turn_board_clockwise function
     * from Board class*/
    ourBoard.turn_board_clockwise();
}


//is_winner
bool Player::is_winner()
{
    /* Function checks if player has won by checking get_game_status */
    return ourBoard.get_game_status() == myPlayer;
}


//get_symbol
char Player::get_symbol() const
{
    /* Getter function for myPlayer */
    return myPlayer;
}