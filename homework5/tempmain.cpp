#include<iostream>
#include"Board.h"
using namespace std;


int main()
{
Board board = Board(4,2);
board.print_board();
cout << endl;
board.put_piece('X', 0);
board.print_board();
cout << endl;
board.turn_board_clockwise();
board.print_board();
cout << endl;
board.put_piece('O', 0);
board.print_board();
cout << endl;
board.turn_board_clockwise();
board.print_board();
cout << endl;
return 0;
}
