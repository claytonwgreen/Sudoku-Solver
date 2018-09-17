#include<iostream>
#include<string>
#include<stdlib.h>
#include<ctime>

using namespace std;

char board[9][9];			// global board that all functions access (can keep global cuz is only 81 chars)
int const total = 45;		// used for telling which number is missing in a row or column (1 + 2 + ... + 9 = 45)
int const squareLocations[] = {0,0, 0,3, 0,6, 3,0, 3,3, 3,6, 6,0, 6,3, 6,6}; // used for finding locations of squares
bool single = false;		// flag if only looking for one square, not whole solution
int singleRow;				// row of single square answer is needed for
int singleCol;				// column of single square answer is needed for

// setting up board
bool 	setup();

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
void 	removeSpaces(string str);


int main() {
	if( !setup() ) { return 1; }

	if( solve() ) {
		if( single ) {
			cout << "the number at row " << singleRow + 1 << " , column " << singleCol + 1 << " is " << board[singleRow][singleCol] << endl << endl;
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

bool setup() {
	string response;
	bool random;
	srand(time(NULL));

	cout << "Would you like the solution the whole puzzle? (y/n)" << endl;
	cin >> response;
	if( response == "y" || response == "yes" ) {}
	else {
		single = true;
		cout << "So you would like to know the number that goes a specific square? (y/n)" << endl;
		cin >> response;
		if( response == "y" || response == "yes" ) {
			cout << "Would you like it to be a random hint (as opposed to a specific square you want to specify)? (y/n)" << endl;
			cin >> response;
			if( response == "y" || response == "yes" ) {
				random = true;
			}
		}
		else { cout << "Well those are the only two things I can do. Goodbye" << endl; return false; }
	}

	cout << endl << "You will now input the sudoku board row by row." << endl << "For empty squares, put a '-' character." << endl;
	if( single && (!random) ) { cout << "For the square you want the number for, put an 'X' character" << endl << "An example of one row of input: \"2--1-X-8-\"" << endl; }
	else { cout << "An example of one row of input: \"2--1---8-\"" << endl; }

	bool foundX = false;
	for(int i = 0; i < 9; i++) {
		bool correctNum = true;
		string res;
		while ( correctNum ) {
			cout << endl << "Please input values for row " << (i + 1) << " below:" << endl;
			cin >> res;
			if( res.size() != 9 ) {
				cout << "Uh oh! Incorrect number of input values. Please try again";
			}
			else { correctNum = false; }
		}
		for(int k = 0; k < 9; k++) {
			char a = res[k];
			if( (a < '0' || a > '9') && a != '-' && a != 'X' ) {
				cout << "Uh oh! Seems like you've input an invalid character. Please try again" << endl;
				i--;
				break;
			} 
			else {
				if( a == 'X' ) {
					singleRow = i;
					singleCol = k;
					board[i][k] = '-';
					foundX = true;
				}
				else { board[i][k] = a; }
			}
		}
	}

	if( random ) {
		do {
			singleRow = rand() % 9;
			singleCol = rand() % 9;
			cout << "row is " << singleRow << " and col is " << singleCol << endl;
		} while ( board[singleRow ][singleCol ] != '-' );
	}
	else if( (!foundX) && single ) {
		cout << "Uh oh! Seems like you haven't placed an 'X' in the playing board." << endl;
		bool valid = true;
		while( valid ) {
			cout << "In which row is the square that you would like the answer to?" << endl;
			cin >> singleRow;
			cout <<  "In which column is the square that you would like the answer to?" << endl;
			cin >> singleCol;
			singleCol--;
			singleRow--;
			if( singleRow < 0 || singleRow > 8 || singleCol < 0 || singleCol > 8 ) {
				cout << "Uh oh! That isn't a valid location. Please try again." << endl;
			}
			else { 
				valid = false; 
			}
		}
	}

	printBoard();

	bool correct = true;
	response = "";
	cout << "Is the above board correct? (y/n)" << endl;
	cin >> response;
	int row, col;
	char num;
	if( response == "y" || response == "yes" ) { correct = false; }
	else { cout << "You will now indicate which squares are incorrect" << endl; }
	while( correct ) {
		cout << "In which row is the value incorrect?" << endl;
		cin >> row;
		cout <<  "In which column is the value incorrect?" << endl;
		cin >> col;
		if( row < 1 || row > 9 || col < 1 || col > 9 ) {
			cout << "Uh oh! That isn't a valid location. Please try again." << endl;
			continue;
		}
		cout << "What value should be in the square at row " << row << " and column " << col << "?" << endl;
		cin >> num;
		if( (num < '0' || num > '9') && num != '-' && num != 'X' ) {
			cout << "Uh oh! That isn't a valid character. Please try again." << endl;
			continue;
		}
		board[row - 1][col - 1] = num;
		printBoard();
		cout << "Is the board now setup correctly? (y/n)" << endl;
		cin >> response;
		if( response == "y" || response == "yes" ) { correct = false; }
	}
	cout << endl;
	return true;
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
	int const NUMSQUARES = 81; 	// sudoku is played on a 9x9 grid which has 81 squares
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

void removeSpaces(string str)
{
    // To keep track of non-space character count
    int count = 0;
 
    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i]; // here count is incremented
    str[count] = '\0';
}












