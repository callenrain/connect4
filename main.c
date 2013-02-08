//
//  main.c
//  Connect4
//
//  Created by Callen Rain on 11/14/12.
//  Copyright (c) 2012 Callen Rain. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>

/*****************************************************************************/
//structure declaration for Game 
struct Game {
	int *board;			// 2d array that contains data describing board cells
   	int current_move;	// column number of the current player's move
	int valid_move;		// (0 or 1) denoting whether current move is valid
	int current_p;		// (1 or 2) denoting which player's turn it is
	int winning_p;		// (1 or 2) denotes winning player if there is a winner
	int winner;			// (0 or 1) 1 if there is a winning move on board
	int computer;		// (0 or 1) 1 if a computer is playing
	int difficulty;		// (4 or 6) recursion depth the computer will use
};

/*****************************************************************************/
//function declarations
struct Game *makeGame();
int play(struct Game *game);
void getMove(struct Game * game);
void enterMove(struct Game * game,int player);
void checkMove(struct Game * game);
void checkWin(struct Game * game);
void printBoard(struct Game * game);
int minMax(int depth, int max_depth,int player,struct Game * game);
struct Game *cloneGame(struct Game * old_game);
int scoreBoard(struct Game * game);
int fullBoard(struct Game * game);

/*****************************************************************************/

int main(void) {
	system("clear");							//clear screen
	//print menu items and let user choose game mode
	printf("WELCOME TO CONNECT 4!\nWOULD YOU LIKE TO:\n\n");
	printf("[1] PLAY AGAINST THE COMPUTER\n");
	printf("[2] PLAY AGAINST ANOTHER HUMAN\n[3] QUIT\n\n");
	int choice;
	scanf("%d",&choice);
	while (choice != 1 && choice != 2 && choice != 3) {
		printf("ENTER A VALID CHOICE!"); //prevent bad input
	} 
	
	if (choice == 3) { //quit
		return 1;
	} 
	
	else if (choice == 1) { //user wants to play against the computer
		struct Game *current_game = NULL;		//initialize game pointer and
		current_game = makeGame();				//obtain created game
		system("clear");							//clear screen
		printf("CHOOSE YOUR DIFFICULTY\n[1] EASY\n[2] HARD\n\n");
		int difficulty;
		scanf("%d",&difficulty); //allow user to pick easy or hard
		while (difficulty != 1 && difficulty != 2) {
			printf("ENTER A VALID CHOICE!");
		}
		current_game -> computer = 1;	//activate computer player
		//set difficulty to 4 for easy and to 6 for hard
		if (difficulty == 1) {current_game -> difficulty = 4;}
		if (difficulty == 2) {current_game -> difficulty = 6;}
		play(current_game);
		} 
		
	else if (choice == 2) { //user wants to play against another human
		struct Game *current_game = NULL;		//initialize game pointer and
		current_game = makeGame();				//obtain created game
		play(current_game);
	}
	

}

/*****************************************************************************/
// input: Game struct pointer		output: int if game is over
//
// this function contains all the code for playing games. a game struct is
// passed in that has all the appropriate settings for gameplay

int play(struct Game *current_game) {
	while (1) {
		printBoard(current_game);			//display board
		getMove(current_game);				//collect move input from player
		checkMove(current_game);			//assert that move is valid
		while(current_game->valid_move == 0) {	//if not valid...
			printBoard(current_game);
			printf("%d is not a valid move!\n",current_game->current_move);
			getMove(current_game);				//get and check new move
			checkMove(current_game);
		}
		//enter validated move into board
		enterMove(current_game,current_game->current_p);
		checkWin(current_game);				//check if winner found
		if (current_game->winner == 1) {	//end game and declare winner
			printBoard(current_game);
			printf("%s %d %s","\nPlayer", current_game->winning_p, "won!\n\n");
			return 1;
		}
		if (current_game->current_p == 1) {	//switch current player
			current_game->current_p = 2;
		}
		else {
			current_game->current_p = 1;
		}
    }
}

/*****************************************************************************/
// input: Game struct pointer		output: none
//
// this function displays the data stored in the board member variable of the
// Game struc in an easy to read format. 

void printBoard(struct Game * game) {
	int i;
	int j;
	system("clear");							//clear screen
	printf("|_0___1___2___3___4___5___6_|\n");	
	for(i=0;i<5;i++) {							//iterate through each row
		printf("|");
		for(j=0;j<7;j++) {
			if (game->board[i*7+j]==1) {		//print 'x' for p1 cells
				printf(" x |");
			} else if (game->board[i*7+j]==2) {	//print 'o' for p1 cells
				printf(" o |");
			} else {
				printf("   |");					//print space for empty cells
			}									// (stored as '0's)
		}
		printf("\n");
	}
	printf("|");
	for(j=0;j<7;j++) {						//print last row with underlines
		if (game->board[5*7+j]==1) {
  			printf("_x_|");
		} else if (game->board[5*7+j]==2) {
			printf("_o_|");
		} else {
			printf("___|");
		}
	}
	printf("\n");
}

/*****************************************************************************/
// input: Game struct pointer		output: none
//
// this function checks the current move and determines if it is a valid move
// for the current board

void checkMove(struct Game * game){
	int *board = game->board;
	int move = game->current_move;
	if (move>=0 && move <=6) {		// make sure move corresponds to one of 
		if (board[move] == 0) {		// the columns and that there is space
			game->valid_move = 1;	// for the move
		}
		else {
			game->valid_move = 0;	// else assert that move is invalid
		}
	}
	else {
		game->valid_move = 0;
	}
}

/*****************************************************************************/
// input: Game struct pointer		output: none
//
// this function prompts the user for their move or calls the minMax
// algorithm for a move at a recursion depth of 4 or 6

void getMove(struct Game * game) {
	int move,diff;
	if (game -> computer == 1 && game -> current_p == 1) {
		printf("\nGetting computer move...\n");
		diff = game -> difficulty;
		game->current_move = minMax(diff,diff,1,game);
	} else {
	printf("%s %d %s","\nPlease enter move for Player",game->current_p,":\n");
	scanf("%d",&move);
	game->current_move=move; 	// set Game variable to the inputted move
	}
}

/*****************************************************************************/
// input: Game struct pointer		output: none
//
// this function enters the current move onto its appropriate spot on the board

void enterMove(struct Game * game,int player) {
	int move = game->current_move;		// define local variables
	int *board = game->board;
	int i;
	
	for(i=0; i<6; i++){					//iterate through row (top to bottom)
        if (board[i*7+move] != 0) {     	// if given cell does not contain a
			board[(i-1)*7+move] = player;	// zero (is empty), set the cell
			return;							// above to the player's move
		}
		else if (i==5) {				// if none of the cells are filled
			board[35+move] = player;	// set the bottom cell to player move
		}
    }
}

/*****************************************************************************/
// input: Game struct pointer		output: none
//
// this function checks all possible winning combinations and sets Game member
// variables winner (whether there is one) and winning_p (which one) to their
// appropriate values

void checkWin(struct Game * game) {
	int *board = game->board;
	int i,j;
	//check vertical wins
	for(j=0; j<7; j++){ // iterate through columns
		// if top 4 vertical cells are identical and non-zero
		if((board[0*7+j] != 0) && (board[0*7+j] == board[1*7+j]) &&
		 	(board[1*7+j] == board[2*7+j]) && (board[2*7+j] == board[3*7+j])) {
			game->winner = 1; // there is a winner
			game->winning_p = board[0*7+j]; // it is the player which holds
		}									// moves at one of the points
		// next set of four cells...
		if((board[1*7+j] != 0) && (board[1*7+j] == board[2*7+j]) && 
			(board[2*7+j] == board[3*7+j]) && (board[3*7+j] == board[4*7+j])) {
			game->winner = 1;
			game->winning_p = board[1*7+j];
		}
		// and four cells that contains the bottom row
		if((board[2*7+j] != 0) && (board[2*7+j] == board[3*7+j]) && 
			(board[3*7+j] == board[4*7+j]) && (board[4*7+j] == board[5*7+j])) {
			game->winner = 1;
			game->winning_p = board[2*7+j];
		}
	}
	
	//check horizontal wins
	for(i=0; i<6; i++){ //iterate through rows
		// check leftmost set of four
		if((board[i*7+0] != 0) && (board[i*7+0] == board[i*7+1]) && 
			(board[i*7+1] == board[i*7+2]) && (board[i*7+2] == board[i*7+3])) {
			game->winner = 1;
			game->winning_p = board[i*7+0];
		} // set of four separated by one cell from the left edge
		if((board[i*7+1] != 0) && (board[i*7+1] == board[i*7+2]) && 
			(board[i*7+2] == board[i*7+3]) && (board[i*7+3] == board[i*7+4])) {
			game->winner = 1;
			game->winning_p = board[i*7+1];
		} // set of four separated by two cells from the left edge
		if((board[i*7+2] != 0) && (board[i*7+2] == board[i*7+3]) &&
			(board[i*7+3] == board[i*7+4]) && (board[i*7+4] == board[i*7+5])) {
			game->winner = 1;
			game->winning_p = board[i*7+2];
		} // set of four touching the right edge
		if((board[i*7+3] != 0) && (board[i*7+3] == board[i*7+4]) &&
			(board[i*7+4] == board[i*7+5]) && (board[i*7+5] == board[i*7+6])) {
			game->winner = 1;
			game->winning_p = board[i*7+3];
		}
	}
	//check diagonal win scenarios
	for(j=0; j<4; j++){	// iterate through columns
		// check downward diagonal wins
			// check diagonal beginning at the top cell
		if((board[0*7+j] != 0) && (board[0*7+j] == board[1*7+j+1]) &&
		 	(board[1*7+j+1] == board[2*7+j+2]) && (board[2*7+j+2] == board[3*7+j+3])) {
			game->winner = 1;
			game->winning_p = board[0*7+j];
		}	// check diagonal beginning at second cell from top
		if((board[1*7+j] != 0) && (board[1*7+j] == board[2*7+j+1]) && 
			(board[2*7+j+1] == board[3*7+j+2]) && (board[3*7+j+2] == board[4*7+j+3])) {
			game->winner = 1;
			game->winning_p = board[1*7+j];
		}	// check diagonal beginning at third cell from top
		if((board[2*7+j] != 0) && (board[2*7+j] == board[3*7+j+1]) && 
			(board[3*7+j+1] == board[4*7+j+2]) && (board[4*7+j+2] == board[5*7+j+3])) {
			game->winner = 1;
			game->winning_p = board[2*7+j];
		}
		//check upward diagonal wins
			// check diagonal beginning at third cell from bottom
		if((board[3*7+j] != 0) && (board[3*7+j] == board[2*7+j+1]) &&
		 	(board[2*7+j+1] == board[1*7+j+2]) && (board[1*7+j+2] == board[0*7+j+3])) {
			game->winner = 1;
			game->winning_p = board[3*7+j];
		}	// check diagonal beginning at second cell from bottom
		if((board[4*7+j] != 0) && (board[4*7+j] == board[3*7+j+1]) && 
			(board[3*7+j+1] == board[2*7+j+2]) && (board[2*7+j+2] == board[1*7+j+3])) {
			game->winner = 1;
			game->winning_p = board[4*7+j];
		}	// check diagonal beginning at bottom cell
		if((board[5*7+j] != 0) && (board[5*7+j] == board[4*7+j+1]) && 
			(board[4*7+j+1] == board[3*7+j+2]) && (board[3*7+j+2] == board[2*7+j+3])) {
			game->winner = 1;
			game->winning_p = board[5*7+j];
		}
	}
	
}

/*****************************************************************************/
// inputs: current recursion depth, max recursion depth, current player on
//		   board, game struct pointer
// output: column of best move if at the highest recursion level, score of
//		   board otherwise
// 
// tries every possible move combination and picks the one that does best for
// the computer and the worst for the human player

int minMax(int depth,int max_depth,int player,struct Game * game) {
	//variable declarations
	int min_score; //assigned the min score from all possible human moves
	int max_score; //assigned the max score from all possible computer moves
	int column; //column with best move 
	struct Game * game_clone; //cloned struct with some move combination
	int next_level; //holds the score of the lower level
	int move_column; //iterates through possible moves
	
	checkWin(game);	//check to see if there is a winner
	if (game->winner == 1) {	// if there is a winner
        if (game->winning_p == 1) {	//if the winner is the computer
			return 99999;	// return large score (good)
		} else if(game->winning_p == 2) {	//if the winner is the human
			return -99999;	//return very small score (bad)
		}
	}

	else if (fullBoard(game) == 1) { //if the board is full
		return 9000;	//return a large score, but not as large as winner
	} else if (depth == 0) { // if we are at the lowest depth
		return scoreBoard(game); //score the board where it is and return that
	} else { //we need to make a choice about which path to choose
		min_score = 100000; //placeholder for a very unlikely min score
		max_score = -100000; //placeholder for a very unlikely max score
		column = 0; //set best column to zero to start
		for (move_column = 0; move_column < 7; move_column++) {
			game_clone = cloneGame(game); //clone game at each column
			game_clone->current_move=move_column; //set move
			checkMove(game_clone);
			if (game_clone->valid_move == 0) {	//if column is full, skip it
				continue;
			}
			enterMove(game_clone,player); //execute move
 	    	if (player == 2) {//if we are deciding options for the human player
				//call minMax to get scores on the lower levels
				next_level = minMax(depth-1,max_depth,1,game_clone); 
				if (next_level < min_score) { //select smallest score
					column = move_column; 
					min_score = next_level; 
				}
			}
			else {//if we are deciding options for the computer player
				//call minMax to get scores on the lower levels
				next_level = minMax(depth-1,max_depth,2,game_clone); 
				if (next_level > max_score) { //select largest score
					column = move_column; 
					max_score = next_level; 
				}
			}
		}
		if (depth == max_depth) { //if at the highest level, just return column
			return column;}
		else if (player == 2) {{return min_score;}} //else just return the score
		else {return max_score;}					//we found
	}
}

/*****************************************************************************/
// input: Game struct pointer		output: Game struct pointer
//
// this function makes a new game and copies all of the information from the
// old game into the new one

struct Game *cloneGame(struct Game * old_game){
	struct Game *new_game;
	int *new_board;
	int *old_board;
	int i,j;
	
	new_game = makeGame(); //create the new game
	//copy all data over
   	new_game -> current_move = old_game -> current_move;
   	new_game -> valid_move = old_game -> valid_move;
   	new_game -> current_p = old_game -> current_p;
   	new_game -> winning_p = old_game -> winning_p;
   	new_game -> winner = old_game -> winner;
	new_game -> computer = old_game -> computer;
	new_game -> difficulty = old_game -> difficulty;
	
	new_board = new_game -> board;
	old_board = old_game -> board;
	
	//set board values to old board values
    for(i=0; i<6; i++){
      for(j=0; j<7; j++){
        new_board[i*7+j] = old_board[i*7+j];
      }
    }
	return new_game;
}

/*****************************************************************************/
// input: Game struct pointer		output: int of board score
//
// this function scores a given game based on how advantageous it is for the
// computer. every arrangement of 4 cells in a row is scored, with cells held
// by the computer getting positive scores and cells held by the human getting
// negative ones

int scoreBoard(struct Game * game) {
	int *w; //array to store cell weights
	int *board = game->board;
	int i,j,k,score;
	
	checkWin(game);	//check to see if there is a winner
	if (game->winner == 1) {	// if there is a winner
        if (game->winning_p == 1) {	//if the winner is the computer
			return 99999;	// return large score (good)
		} else if(game->winning_p == 2) {	//if the winner is the human
			return -99999;	//return very small score (bad)
		}
	}
	
	score = 0;
	w = (int *)malloc(sizeof(int)*6*7); //allocate memory for weighting array
	
	if(!w) { // error statement in case of malloc failure
    	printf("weights malloc failed\n");
    	exit(1);
    }
    for(i=0; i<6; i++){ //set cell weights to IBEF board weights
						//cells closer to the center are weighted more heavily
		if (i==0||i==5) {w[i*7+0]=3;w[i*7+1]=4;w[i*7+2]=5;w[i*7+3]=7;
				         w[i*7+4]=5;w[i*7+5]=4;w[i*7+6]=3;}
		if (i==1||i==4) {w[i*7+0]=4;w[i*7+1]=6;w[i*7+2]=8;w[i*7+3]=10;
				         w[i*7+4]=8;w[i*7+5]=6;w[i*7+6]=4;}
		if (i==2||i==3) {w[i*7+0]=5;w[i*7+1]=8;w[i*7+2]=11;w[i*7+3]=13;
				         w[i*7+4]=11;w[i*7+5]=8;w[i*7+6]=5;}}

	//check horizontal scores
	for(i=0; i<6; i++){ //iterate through rows
		for(j=0; j<4; j++){ //iterate through columns
			for(k=0; k<4; k++){
				if(board[i*7+j+k] == 1) { //if cell is held by the computer
					score += w[i*7+j+k];  //add a positive score
				} else if(board[i*7+j+k] == 0) { //if no one holds the cell
					score += 0;					
				} else {	//the human holds the cell
					score += w[i*7+j+k]*-1;	//add negative weight
				}
			}
		}
	}
	
	//check vertical scores
	for(j=0; j<6; j++){ //iterate through columns
		for(i=0; i<4; i++){ //iterate through rows
			for(k=0; k<4; k++){
				if(board[(i+k)*7+j] == 1) {
					score += w[(i+k)*7+j];
				} else if(board[(i+k)*7+j] == 0) {
					score += 0;
				} else {
					score += w[(i+k)*7+j]*-1;
				}
			}
		}
	}
	
	//check up diagonals	
    for(i=3; i<6; i++){ //iterate through rows
		for(j=0; j<4; j++){ //iterate through columns
			for(k=0; k<4; k++){
				if(board[(i-k)*7+j+k] == 1) {
					score += w[(i-k)*7+j+k];
				} else if(board[(i-k)*7+j+k] == 0) {
					score += 0;
				} else {
					score += w[(i-k)*7+j+k]*-1;
				}
			}
		}
	}
		
	//check down diagonals
	for(i=0; i<3; i++){ //iterate through rows
		for(j=0; j<4; j++){ //iterate through columns
			for(k=0; k<4; k++){
				if(board[(i+k)*7+j+k] == 1) {
					score += w[(i+k)*7+j+k];
				} else if(board[(i+k)*7+j+k] == 0) {
					score += 0;
				} else {
					score += w[(i+k)*7+j+k]*-1;
				}
			}
		}
	}
	return score;
}

/*****************************************************************************/
// input: Game struct pointer		output: int 1 if board is full
//
// this function determines whether a given game board is full

int fullBoard(struct Game * game) {
	int *board = game->board;
	int j;
	//check to see if the top cells of each column are occupied
	if (board[0] != 0 && board[1] != 0 && board[2] != 0 && board[3] != 0 &&
	    board[4] != 0 && board[5] != 0 && board[6] != 0) {
			return 1;
	}
	return 0;
}

/*****************************************************************************/
// input: none		output: Game struct pointer
//
// this function allocates memory for the Game structure and initializes
// its variables

struct Game *makeGame(){
	// local variable initializations
	int *current_board;
	struct Game *current_game = NULL;
	int move, current_player, winner, winning_player,valid,computer,difficulty;
	winning_player = 0;
	winner = 0;
	move = 0;
	current_player = 1;
	valid = 1;
	computer = 0;
	difficulty = 0;
	
	// allocate memory for board data member
	current_board = (int *)malloc(sizeof(int)*6*7);
	if(!current_board) { // error statement in case of malloc failure
    	printf("current_board malloc failed\n");
    	exit(1);
    }

	// allocate memory for Game structure
	current_game = (struct Game*)malloc(sizeof(int)*6*7+sizeof(int)*3);
	if(!current_game) {	// error statement in case of malloc failure
    	printf("current_game malloc failed\n");
    	exit(1);
    }
	
	// initialize board array values to 0
    int i,j;
    for(i=0; i<6; i++){
      for(j=0; j<7; j++){
        current_board[i*7+j] = 0;
      }
    }
	
	// set Game member variables to local variables defined above
	current_game->board=current_board;
	current_game->current_move = move;
	current_game->current_p = current_player;
	current_game->winner = winner;
	current_game->winning_p = winning_player;
	current_game->valid_move = valid;
	current_game->computer = computer;
	current_game->difficulty = difficulty;
	return current_game;	// return pointer to created game
}