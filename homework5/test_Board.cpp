#include "Board.h"
#include <iostream>
using namespace std;

int main(){
// Test case 1: No winner
Board board1(3, 3);
board1.put_piece('X', 0);
board1.put_piece('O', 1);
board1.put_piece('X', 2);
board1.put_piece('O', 0);
board1.put_piece('X', 1);
board1.put_piece('O', 2);
board1.put_piece('O', 0);
board1.put_piece('O', 1);
board1.put_piece('X', 2);
board1.print_board();
char result1 = board1.get_game_status(); // Expected: '-'
cout << result1 << endl;

// Test case 2: X wins horizontally
Board board2(3, 3);
board2.put_piece('X', 0);
board2.put_piece('X', 1);
board2.put_piece('X', 2);
board2.put_piece('O', 0);
board2.put_piece('O', 1);
board2.put_piece('X', 0);
board2.put_piece('O', 1);
board2.put_piece('O', 2);
board2.put_piece('X', 2);
board2.print_board();
char result2 = board2.get_game_status(); // Expected: 'X'
cout << result2 << endl;

// Test case 3: O wins vertically
Board board3(3, 3);
board3.put_piece('X', 0);
board3.put_piece('O', 1);
board3.put_piece('X', 2);
board3.put_piece('O', 0);
board3.put_piece('O', 1);
board3.put_piece('O', 2);
board3.put_piece('X', 0);
board3.put_piece('X', 1);
board3.put_piece('O', 2);
board3.print_board();
char result3 = board3.get_game_status(); // Expected: 'O'
cout << result3 << endl;

// Test case 4: Draw
Board board4(3, 3);
board4.put_piece('X', 0);
board4.put_piece('O', 1);
board4.put_piece('X', 2);
board4.put_piece('O', 0);
board4.put_piece('X', 1);
//board4.put_piece('O', 2);
board4.put_piece('O', 0);
board4.put_piece('X', 1);
//board4.put_piece('O', 2);
board4.print_board();
board4.turn_board_clockwise();
board4.print_board();
board4.turn_board_clockwise();
board4.print_board();
board4.turn_board_clockwise();
board4.print_board();
board4.turn_board_clockwise();
board4.print_board();
board4.turn_board_clockwise();
board4.print_board();
board4.turn_board_clockwise();
board4.print_board();
board4.turn_board_clockwise();
board4.print_board();
char result4 = board4.get_game_status(); // Expected: 'D'
cout << result4 << endl;


return 0;
}