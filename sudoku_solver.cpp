#include<iostream>
using namespace std;

int const NUMSQUARES = 81; 	// sudoku is played on a 9x9 grid which has 81 squares
char board[9][9];			// global board that all functions access (can keep global cuz is only 81 chars)
int const total = 45;		// used for telling which number is missing in a row or column (1 + 2 + ... + 9 = 45)
int const squareLocations[] = {0,0, 0,3, 0,6, 3,0, 3,3, 3,6, 6,0, 6,3, 6,6};


// helper functions
int 	integerify(char a);					// turn a char number into an int
void 	printBoard();						// print out board
int 	columnOffset(int column);			
bool 	checkCompletion();					// check if entire board is filled in
int 	getSquare(int row, int column);
void 	increment(int* row, int* column);
void 	decrement(int* row, int* column);


bool	checkColumnForNumber(int column, char number);
bool 	checkRowForNumber(int row, char number);
bool 	checkSquareForNumber(int square, char number);

bool 	crosshatch();
bool 	crosshatchNumberInSquare(int square, char num);

void 	backtrack();
bool 	checkIfValid(char num, int row, int col);




bool 	solve();							// function which calls other functions until puzzle solved







int main( int argc, char *argv[] ) {
	//cout << "number of inputs is " << argc - 1 << endl;

	// check if input is a full board worth of values ('-' means blank)
	if( (argc - 1) !=  NUMSQUARES) {
		cout << "incorrect number of input values" << endl;
		return 1;
	}

	// fill out board with inputted values
	for(int i = 0; i < NUMSQUARES; i++) {
		int row = i/9;
		board[row][i - (row * 9)] = *argv[i + 1];
	}

	cout << "the starting board is:" << endl;
	printBoard();

	if( solve() ) {
		cout << "the finished board is:" << endl;
		printBoard();
	}
	else {
		cout << endl << endl << "   ¯\\_(ツ)_/¯" << endl << "you've stumped me" << endl << endl << "this is how far I got:" << endl;
		printBoard();
	}
}

bool solve() {
	while( true ) {
		if ( crosshatch() ) {}
		else if ( checkCompletion() ) { return true; }
		//else return false;
		else { backtrack(); break; }
		//backtrack();
		cout << "one iteration" << endl;
	}

	if ( checkCompletion() ) { return true; }
	else { return false; }
}


void backtrack() {
	cout << "the board before bactracking is:" << endl;
	printBoard();

	char originalBoard[9][9];

	for(int i = 0; i < 9; i++) {
		for(int k = 0; k < 9; k++) {
			originalBoard[i][k] = board[i][k];
		}
	}

	int row = 0;
	int col = 0;
	bool foundErrorOnThisPath = false;

	while (true) {
		if( row == 9 ) { return; }

		if(originalBoard[row][col] != '-') {
			if( foundErrorOnThisPath ) {
				decrement(&row, &col);
			}
			else {
				increment(&row, &col);
			}
			continue;
		}

		if( foundErrorOnThisPath ) {
			if( originalBoard[row][col] != '-' ) {
				decrement(&row, &col);
				continue;
			}

			foundErrorOnThisPath = false;
			bool found = false;
			for(char a = board[row][col]; a <= '9'; a += 1) {
				cout << "updating space using: " << a << " at " << row << ", " << col << endl;
				if( checkIfValid(a, row, col) ) {
					board[row][col] = a;
					found = true;
					break;
				}
			}
			if( !found ) {
				board[row][col] = '-';
				decrement(&row, &col);
				foundErrorOnThisPath = true;
			}
			else {
				increment(&row, &col);
				foundErrorOnThisPath = false;
			}
		}

		else {
			bool found = false;
			printBoard();
			for(char a = '1'; a <= '9'; a += 1) {
				cout << "testing new space: trying " << a << " at " << row << ", " << col << endl;
				if( checkIfValid(a, row, col) ) {
					board[row][col] = a;
					found = true;
					break;
				}
			}
			if( !found ) {
				board[row][col] = '-';
				decrement(&row, &col);
				foundErrorOnThisPath = true;
			}
			else {
				cout << "found working solution for " << row << "," << col;
				increment(&row, &col);
				cout << " now searching at " << row << "," << col << endl;
 				foundErrorOnThisPath = false;
			}
		}
	}
}

void increment(int* row, int* column) {
	*column += 1;
	if( *column == 9 ) {
		*row += 1;
		*column = 0;
	}
}

void decrement(int* row, int* column) {
	*column -= 1;
	if( *column == -1) {
		*row -= 1;
		*column = 8;
	}

}

bool checkIfValid(char num, int row, int col) {
	if( checkColumnForNumber(col, num) || checkRowForNumber(row, num) || checkSquareForNumber(getSquare(row, col), num) ) {
		return false;
	}
	else {
		return true;
	}
}















bool crosshatch() {
	bool worked = false;
	for(int i = 0; i < 9; i++) {
		for(char k = '1'; k <= '9'; k = k + 1) {
			if( crosshatchNumberInSquare(i, k) ) {
				worked = true;
			}
		}
	}
	return worked;
}

bool crosshatchNumberInSquare(int square, char num) {
	if( checkSquareForNumber( square, num ) ) {
		return false;
	}
	else {
		int row_offset = 3 * (square/3);
		int column_offset = 3 * (columnOffset(square));
		char temp[3][3];
		for(int i = 0; i < 3; i++) {
			for(int k = 0; k < 3; k++) {
				temp[i][k] = board[row_offset + i][column_offset + k];
			}
		}
		for(int i = 0; i < 3; i++) {
			for(int k = 0; k < 3; k++) {
				if(checkColumnForNumber(column_offset + k, num)) {
					temp[i][k] = 'x';
				}
				if(checkRowForNumber(row_offset + i, num)) {
					temp[i][k] = 'x';
				}
			}
		}
		int count = 0, row_pos, col_pos;

		for(int i = 0; i < 3; i++) {
			for(int k = 0; k < 3; k++) {
				if(temp[i][k] == '-') {
					count++;
					row_pos = i;
					col_pos = k;
				}
			}
		}
		if(count == 1) {
			board[row_offset + row_pos][column_offset + col_pos] = num;
			return true;
		}
		else { return false; }
	}
}

bool checkColumnForNumber(int column, char number) {
	for(int i = 0; i < 9; i++) {
		if( board[i][column] == number) {
			return true;
		}
	}
	return false;
}

bool checkRowForNumber(int row, char number) {
	for(int i = 0; i < 9; i++) {
		if( board[row][i] == number) {
			return true;
		}
	}
	return false;
}

bool checkSquareForNumber(int square, char number) {
	int row = squareLocations[2 * square];
	int column = squareLocations[2 * square +1];
	for(int i = 0; i < 3; i++) {
		for(int k = 0; k < 3; k++) {
			if( board[row + i][column + k] == number ) {
				return true;
			}			
		}
	}
	return false;
}

int getSquare(int row, int column) {
	if(0 <= row && row < 3 && 0 <= column && column < 3) { return 0; }
	else if(0 <= row && row < 3 && 3 <= column && column < 6) { return 1; }
	else if(0 <= row && row < 3 && 6 <= column && column < 9) { return 2; }
	else if(3 <= row && row < 6 && 0 <= column && column < 3) { return 3; }
	else if(3 <= row && row < 6 && 3 <= column && column < 6) { return 4; }
	else if(3 <= row && row < 6 && 6 <= column && column < 9) { return 5; }
	else if(6 <= row && row < 9 && 0 <= column && column < 3) { return 6; }
	else if(6 <= row && row < 9 && 3 <= column && column < 6) { return 7; }
	else if(6 <= row && row < 9 && 6 <= column && column < 9) { return 8; }
	else return -1;
}

int columnOffset(int column) {
	switch (column) {
		case 0: return 0;
		case 1: return 1;
		case 2: return 2;
		case 3: return 0;
		case 4: return 1;
		case 5: return 2;
		case 6: return 0;
		case 7: return 1;
		case 8: return 2;
		default: return -1;
	}
}

int integerify(char a) {
	return ( a - '0');
}

bool checkCompletion() {
	for(int i = 0; i < 9; i++) {
		for(int k = 0; k < 9; k++) {
			if( board[i][k] == '-' ) {
				return false;
			}
		}
	}
	return true;
}

void printBoard() {
	cout << endl << "-------------------------------------" << endl << "| ";
	for(int i = 0; i < NUMSQUARES; i++) {
		int row = i/9;
		cout << board[row][(i - (row * 9))]; 
		if( i - (row * 9) == 8) {
			if( i == 80) {
				cout << " |" << endl << "-------------------------------------" << endl << endl;
			}
			else {
				cout << " |" << endl << "-------------------------------------" << endl << "| ";
			}
		}
		else {
			cout << " | ";
		}
	}
}









