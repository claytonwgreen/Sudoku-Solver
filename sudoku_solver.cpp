#include<iostream>
#include<string>

using namespace std;

int const NUMSQUARES = 81; 	// sudoku is played on a 9x9 grid which has 81 squares
char board[9][9];			// global board that all functions access (can keep global cuz is only 81 chars)
int const total = 45;		// used for telling which number is missing in a row or column (1 + 2 + ... + 9 = 45)
int const squareLocations[] = {0,0, 0,3, 0,6, 3,0, 3,3, 3,6, 6,0, 6,3, 6,6};

// crosshatching
bool 	crosshatch();
bool 	crosshatchNumberInSquare(int square, char num);

// simple columns
bool 	checkColumnsSimple();
int 	checkColumnsForSingleMissing();

// simple rows
bool 	checkRowsSimple();
int 	checkRowsForSingleMissing();

// simple squares
bool 	checkSquares();
int 	checkSquaresForSingleMissing();
void 	fillInSquare(int square);
bool 	checkSquareAlmostFull(int row, int column);

// backtracking
void 	backtrack();

// handles using the different methods above
bool 	solve();				

// helper functions
void 	printBoard();									// print out board
bool 	checkCompletion();								// check if entire board is filled in
bool 	checkCorrectCompletion();						// ensure solution is valid
int 	integerify(char a);								// turn a char number into an int
int 	columnOffset(int column);						// return column offset for crosshatching
int 	getSquare(int row, int column);					// return square based on row and column
void 	increment(int* row, int* column);				// function to increment rows and columns
void 	decrement(int* row, int* column);				// function to decrement rows and columns
bool	checkColumnForNumber(int column, char number);	// check if 'number' is found in the column
bool 	checkRowForNumber(int row, char number);		// check if 'number' is found in the row
bool 	checkSquareForNumber(int square, char number);	// check if 'number' is found in the square
bool 	checkIfValid(char num, int row, int col);		// check if 'number' is allowed to be at row, col


int main( int argc, char *argv[] ) {
	uint singleRow;
	uint singleCol;
	bool single = false;
	string response;

	cout << "Would you like the solution the whole puzzle? (y/n)" << endl;
	cin >> response;
	if( response == "y" || response == "yes" ) { 
		single = true; 
	}
	else {
		cout << "So you would like to know the number that goes a specific square? (y/n)" << endl;
		cin >> response;
		if( response == "y" || response == "yes" ) {}
		else { cout << "Well those are the only two things I can do. Goodbye" << endl; return 0; }
	}

	cout << "While inputting values for each row, you can place spaces between each number. For empty squares, put a '-' character."
	if( single ) { cout << "For the square you want the number for, put an 'X' character" }
	cout << endl;

	for(int i = 0; i < 9; i++) {
		cout << "Input values for row " << (i + 1) << endl;
		cin >> response;
		remove_if(response.begin(), response.end(), isspace);
		for(int k = 0; k < 9; k++) {
			char a = response[k];
			if( (a < '0' || a > '9') && a != '-' && a != 'X' ) {
				cout << "Uh oh! Seems like you've input an invalid character. Please try again" << endl;
				i--;
				break;
			} 
			else {
				board[i][k] = a;
			}
		}
	}



	// fill out board with inputted values
	for(int i = 0; i < NUMSQUARES; i++) {
		int row = i/9;
		board[row][i - (row * 9)] = *argv[i + 1];
		if( *argv[i + 1] == 'X' ) {
			singleRow = row;
			singleCol = (i - (row * 9));
			single = true;
			board[singleRow][singleCol] = '-';
		}
	}

	cout << "the starting board is:" << endl;
	printBoard();

	if( solve() ) {
		if( single ) {
			cout << "the number at row " << singleRow + 1 << " , column " << singleCol + 1 << " is " << board[singleRow][singleCol] << endl;
		}
		else {
			cout << "the finished board is:" << endl;
			printBoard();
		}
	}
	else {
		cout << endl << endl << "   ¯\\_(ツ)_/¯" << endl << "you've stumped me" << endl << endl << "this is how far I got:" << endl;
		printBoard();
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

bool checkColumnsSimple() {
	int column = checkColumnsForSingleMissing();
	if( column != -1 ) {
		int count = 0;
		int position = -1;
		for(int i = 0; i < 9; i++) {
			if( board[i][column] != '-') {
				count += integerify(board[i][column]);
			}
			else {
				position = i;
			}
		}
		int number = (total - count);
		cout << "the number is " << number;
		board[position][column] = '0' + number;
		return true;
	}
	return false;
}

int checkColumnsForSingleMissing() {
	for(int i = 0; i < 9; i++) {
		int count = 0;
		for(int k = 0; k < 9; k++) {
			if( board[k][i] != '-' ) {
				count++;
			}
		}
		if( count == 8){
			return i;
		}
	}
	return -1;
}



bool checkRowsSimple() {
	int row = checkRowsForSingleMissing();
	if( row != -1 ) {
		int count = 0;
		int position = -1;
		for(int i = 0; i < 9; i++) {
			if( board[row][i] != '-') {
				count += integerify(board[row][i]);
			}
			else {
				position = i;
			}
		}
		int number = (total - count);
		board[row][position] = '0' + number;
		return true;
	}
	return false;
}

int checkRowsForSingleMissing() {
	for(int i = 0; i < 9; i++) {
		int count = 0;
		for(int k = 0; k < 9; k++) {
			if( board[i][k] != '-' ) {
				count++;
			}
		}
		if( count == 8){
			return i;
		}
	}
	return -1;
}

bool checkSquares() {
	int square = checkSquaresForSingleMissing();
	if( square == -1 ) {
		return false;
	}
	else {
		fillInSquare(square);
		return true;
	}
}

int checkSquaresForSingleMissing() {
	int count = 0;
	for(int i = 0; i < 18; i += 2) {
		if( checkSquareAlmostFull(squareLocations[i], squareLocations[i + 1]) ) {
			return count;
		}
		count++;
	}
	return -1;
}	

bool checkSquareAlmostFull(int row, int column) {
	int count = 0;
	for(int i = 0; i < 3; i++) {
		for(int k = 0; k < 3; k++) {
			if( board[row + i][column + k] != '-' ) {
				count++;
			}			
		}
	}
	if( count == 8 ) { return true; }
	else { return false; }
}

void fillInSquare(int square) {
	int count = 0;
	int row = squareLocations[2 * square];
	int column = squareLocations[2 * square + 1];
	int position_row;
	int position_column;
	for(int i = 0; i < 3; i++) {
		for(int k = 0; k < 3; k++) {
			if( board[row + i][column + k] != '-' ) {
				count += integerify(board[row + i][column + k]);
			}	
			else {
				position_row = row + i;
				position_column = column + k;
			}		
		}
	}
	char answer = '0' + (total - count);
	board[position_row][position_column] = answer;
}

void backtrack() {
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

		if( row == 9 || row == -1 ) { return; }

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
			for(char a = '1'; a <= '9'; a += 1) {
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
	}
}

bool solve() {
	while( true ) {
		if ( crosshatch() ) {}
		else if ( checkCompletion() && checkCorrectCompletion() ) { return true; }
		else if ( checkRowsSimple() ) {}
		else if ( checkColumnsSimple() ) {}
		else if ( checkSquares() ) {}
		else { 
			backtrack(); 
			if ( checkCompletion() ) {
				if( checkCorrectCompletion() ) { return true; }
				else { return false; }
			}
			else { return false; }
		}
	}
}

bool checkCorrectCompletion() {
	for(int i = 0; i < 9; i++) {
		for(int k = 0; k < 9; k++) {
			char num = board[i][k];
			board[i][k] = '-';
			if(! checkIfValid(num, i, k) ) {
				return false;
			}
			board[i][k] = num;
		}	
	}
	return true;
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

void ClearScreen() {
	cout << string( 100, '\n' );
}















