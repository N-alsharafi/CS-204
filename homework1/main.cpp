//CS-204 assignment number-1 by Nuh Al-Sharafi

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

//function declarations

ifstream get_valid_file();
vector<vector<char>> get_matrix();
bool valid_matrix(const vector<vector<char>> & mat);
bool valid_coordinates(const vector<vector<char>>& mat, int row, int col);
bool valid_char(const char & letter);
bool valid_moves(const vector<vector<char>> & mat);
bool empty_matrix(const vector<vector<char>> & mat);
bool game_continues(const vector<vector<char>> & mat);
void print_matrix(const vector<vector<char>> & mat);
int play_game(vector<vector<char>> & mat);
int move_tile(vector<vector<char>> & mat, int row, int col);
void swap_tiles(vector<vector<char>> & mat, int mr, int mc, int nr, int nc);

//implement a function to move in every direction, before each swap, it checks if it can drop the element




int main() {
    /* processing matrix */

    //open the file and read into matrix
    vector<vector<char>> matrix = get_matrix();

    //check if matrix is valid
    if (!valid_matrix(matrix)) {
        cout << "Erroneous file content. Program terminates.";
        return 0;
    }

    //start game
    cout << "The matrix file contains:\n";
    print_matrix(matrix);
    return play_game(matrix);
}


ifstream get_valid_file() {
    /* Function prompts user for filename repeatedly
     * until a valid filename is entered.
     * returns file-content
     */
    string filename;
    ifstream matrix;

    cout << "Please enter the file name:\n";
    cin >> filename;

    matrix.open(filename.c_str());

    while(matrix.fail()) {
        cout << "Could not open the file. Please enter a valid file name:\n";
        cin >> filename;

        matrix.open(filename.c_str());
    }

    return matrix;
}


vector<vector<char>> get_matrix() {
    /* This function gets the text file name from the user,
     * Opens the text file (using get_valid_file()) and reads it
     * into a matrix and returns it.
     * */
    ifstream text_matrix = get_valid_file();
    vector<vector<char>> matrix;

    // reading the text file into a matrix
    string line;
    while(getline(text_matrix, line)) {
        istringstream line_stream(line);
        char ch;
        vector<char> row;

        while(line_stream.get(ch)) {
            row.push_back(ch);
        }

        matrix.push_back(row);
    }

    return matrix;
}


bool valid_matrix(const vector<vector<char>> & mat) {
    /* Function checks if input is an actual matrix (consistent
     * number of chars) and if the chars are of the 5 allowed types:
     *  u, d, l, r, -
     * */

    //check matrix dimensions
    for (int i=0; i< mat.size(); i++) {
        if ( mat[0].size() != mat[i].size()) {
            return false;
        }
    }

    //check matrix contents
    for (int i=0; i<mat.size(); i++) {
        for (int j=0; j<mat[0].size(); j++) {
            if(!valid_char(mat[i][j])){
                return false;
            }
        }
    }
    return true;
}


bool valid_char(const char & letter){
    /*Function checks if the char is among the allowed chars
     * */
    char valid[5] = {'u', 'd', 'l', 'r', '-'};
    for (int i = 0; i< 5; i++) {
        if (valid[i] == letter) {
            return true;
        }
    }
    return false;

}


void print_matrix(const vector<vector<char>> & mat) {
    /* This function prints the matrix on the screen
     * */
    for(int i=0; i< mat.size(); i++) {
        for(int j=0; j< mat[0].size(); j++){
            cout << mat[i][j];
        }
        cout << endl;
    }
}


int play_game(vector<vector<char>> & mat) {
    /* Function will handle the running of the game
     * */
    while(game_continues(mat)) {

        //get input
        int row_coord, col_coord;
        cout << "Please enter the row and column of the tile that you want to move:\n";
        cin >> row_coord >> col_coord;

        //check input
        while(!valid_coordinates(mat, row_coord, col_coord)) {
            cout << "Invalid row or column index.\n";
            cout << "Please enter the row and column of the tile that you want to move:\n";
            cin >> row_coord >> col_coord;
        }

        //implement move & print message
        int no_of_moves = move_tile(mat, row_coord, col_coord);

        if (no_of_moves == 0) {
            cout << "No tiles have been moved.\n";
        }
        else {
            cout << "Tile at (" <<row_coord<<","<<col_coord<<") has been moved.\n";
        }

        //print matrix
        print_matrix(mat);
    }
    if (empty_matrix(mat)){
        cout << "The matrix is empty.";
    } else if (!valid_moves(mat)) {
        cout << "No more moves are possible.";
    }

    //print game ending message
    return 0;
}


bool valid_moves(const vector<vector<char>> & mat){
    /* Function checks if there are valid moves left to be made
     * returns true if there are
     * */
    bool moves = false; //assumes no moves left

    for (int i=0; i<mat.size();i++) {
        for (int j=0; j<mat[0].size();j++) {
            if (mat[i][j] != '-') {

                if (mat[i][j] == 'u') {
                    //if tile is u, check if its top row
                    if (i==0) {
                        moves = true;
                    }
                    //check if 'u' can move at least one tile
                    else if (mat[i-1][j] == '-') {
                        moves =  true;
                    }
                }

                else if (mat[i][j] == 'd') {
                    //if tile is d, check if its bottom row
                    if (i==mat.size()-1) {
                        moves = true;
                    }
                    // check if 'd' can move at least one tile
                    else if (mat[i+1][j] == '-') {
                        moves = true;
                    }
                }

                else if (mat[i][j] == 'l') {
                    //if tile is l, check if its left-most column
                    if (j == 0) {
                        moves = true;
                    }
                    // check if 'l' can move at least one tile
                    else if (mat[i][j-1]=='-') {
                        moves = true;
                    }
                }

                else if (mat[i][j] == 'r') {
                    //if tile is r, check if its right-most column
                    if (j==mat[0].size()-1) {
                        moves = true;
                    }
                    // check if 'r' can move at least one tile
                    else if (mat[i][j+1]=='-') {
                        moves = true;
                    }

                }
            }
        }
    }
    return moves;
}


bool empty_matrix(const vector<vector<char>> & mat) {
    /* Function checks if the matrix is empty, return true if it is
     * */
    for (int i=0; i< mat.size(); i++) {
        for (int j=0; j< mat[0].size(); j++) {
            if ('-' != mat[i][j]) {
                return false;
            }
        }
    }
    return true;
}


bool game_continues(const vector<vector<char>> & mat){
    /* Function checks if the game ending conditions are not satisfied
     * returns true if the game is supposed to continue
     * */
    if (valid_moves(mat) && !empty_matrix(mat)) {
        return true;
    }
    return false;
}


bool valid_coordinates(const vector<vector<char>>& mat, int row, int col) {
    /* Function checks if coordinates are valid
     * returns true if they are
     * */

    if (row >= 0 && row < mat.size()){
        if (col >=0 && col < mat[0].size()) {
            return true;
        }
    }
    return false;
}


int move_tile(vector<vector<char>> & mat, int row, int col) {
    /* Function checks what character is at the position the user entered
     * Then moves the tile accordingly.
     * */
    int moves = 0;

    if (mat[row][col] == '-') {
        return moves;
    } else if (mat[row][col] == 'u') {

        //continuously check and move u until we hit another character or drop off the board
        while (row != 0) {
            if (mat[row-1][col] == '-') {
                swap_tiles(mat, row, col, row - 1, col);
                moves++;
                row--;
            }
            else {
                //another character was hit, return moves.
                return moves;
            }
        }
        mat[row][col] = '-';
        moves++;

    } else if (mat[row][col] == 'd') {

        //same as u
        while(row != mat.size()-1) {
            if (mat[row+1][col] == '-') {
                swap_tiles(mat, row, col, row + 1, col);
                moves++;
                row++;
            }
            else {
                //another character was hit
                return moves;
            }
        }
        mat[row][col] = '-';
        moves++;

    } else if (mat[row][col] == 'l') {

        //same as d
        while(col != 0) {
            if (mat[row][col-1] == '-') {
                swap_tiles(mat, row, col, row, col - 1);
                moves++;
                col--;
            }
            else {
                //another character was hit
                return moves;
            }
        }
        mat[row][col] = '-';
        moves++;

    } else if (mat[row][col] == 'r') {

        //same as l
        while(col != mat[0].size()-1) {
            if (mat[row][col+1] == '-') {
                swap_tiles(mat, row, col, row, col + 1);
                moves++;
                col++;
            }
            else {
                //another character was hit
                return moves;
            }
        }
        mat[row][col] = '-';
        moves++;

    }

    return moves;
}


void swap_tiles(vector<vector<char>> & mat, int mr, int mc, int nr, int nc){
    /* Function is supposed to swap tiles, usually a dash with a letter value
     * */
    char temp = mat[mr][mc];
    mat[mr][mc] = mat[nr][nc];
    mat[nr][nc] = temp;
}