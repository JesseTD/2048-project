/*
 * 2048.cpp
 *
 *  Created on: Jul 18, 2016
 *      Author: Jesse
 */

#include <iostream>
//Needed for random number gen
#include <stdlib.h>
//Needed for random number seed
#include <time.h>
//Needed for filer write/read
#include <fstream>
//Used in file reading
#include <string>
//GUI like library to create a digital screen, color, and accepting in arrow keys
#include "curses.h"

using namespace std;


class Game{
private:
	int score;
	//Game size. 4x4 through 8x8
	int size;
	//Game board self limited by size during operations
	int game[8][8];
	//Number of tiles current filling board. Max is size^2
	int currentTiles;
	//num2048 and hasWon used in conjunction to show message when first 2048 is achieved
	int num2048;
	bool hasWon;
	//boolean to show if game has ended do to losing or quitting
	bool cont;

	//adds a tile (2 or 4) to a random position on the bored
	void addTile(){
		//counter
		int unfilledPos = 0;
		//chooses a random unfilled position on board to fill
		int randPosToFill = (rand() % ((size*size) - currentTiles))+1;

		//for loop to increase unfilledPos until it is  equal to randPosToFill
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++){
				//When found place a random number in the position
				if(game[i][j]==0){
					unfilledPos++;
					if(unfilledPos==randPosToFill){
						if((rand()%10) < 9){
							game[i][j] = 2;
						}
						else{
							game[i][j] = 4;
						}
					}
				}
			}
		}
		//Increase the counter for number of tiles on the board
		currentTiles += 1;
	}

	//accepts a valid key and returns a character corresponding to it for use
	char acceptKey(){
		int key;
		while(true){
			//curses method to grab keys and return corresponding ints, including special keys
			key = getch();
			switch(key){
			case KEY_UP:
				return 'w';
			case KEY_DOWN:
				return 's';
			case KEY_LEFT:
				return 'a';
			case KEY_RIGHT:
				return 'd';
			case 's':
				return 'k';
			case 'q':
				return 'q';
			default:
				cout << "Please input valid key. Arrow keys for movement," << endl;
				cout << "s to save and quit game, and q to end game without saving" << endl;
			}
		}
	}

	//Modifies game within size constraints then adds a new tile
	void swipeUp(){
		// Push up
		for(int r = 0; r < size - 1; r++){
			for(int i = 0; i < size - 1; i++){
				for(int j = 0; j < size; j++){
					if(game[i][j]==0){
						game[i][j] = game[i+1][j];
						game[i+1][j] = 0;
					}
				}
			}
		}
		// Do all possible additions/combinations
		for(int i = 0; i < size - 1; i++){
			for(int j = 0; j < size; j++){
				if(game[i+1][j]==game[i][j] && game[i][j] != 0){
					game[i][j] += game[i+1][j];
					if(game[i][j] == 2048){
						num2048++;
					}
					game[i+1][j] = 0;
					score += game[i][j];
					currentTiles--;
				}
			}
		}
		// Re-push up after the additions
		for(int r = 0; r < size - 1; r++){
			for(int i = 0; i < size - 1; i++){
				for(int j = 0; j < size; j++){
					if(game[i][j]==0){
						game[i][j] = game[i+1][j];
						game[i+1][j] = 0;
					}
				}
			}
		}
		addTile();
	}

	//Modifies game within size constraints then adds a new tile
	void swipeDown(){
		// Push down
		for(int r = 0; r < size - 1; r++){
			for(int i = size - 1; i > 0; i--){
				for(int j = 0; j < size; j++){
					if(game[i][j]==0){
						game[i][j] = game[i-1][j];
						game[i-1][j] = 0;
					}
				}
			}
		}
		// Do all possible additions/combinations
		for(int i = size - 1; i > 0; i--){
			for(int j = 0; j < size; j++){
				if(game[i-1][j]==game[i][j] && game[i][j] != 0){
					game[i][j] += game[i-1][j];
					if(game[i][j] == 2048){
						num2048++;
					}
					game[i-1][j] = 0;
					score += game[i][j];
					currentTiles--;
				}
			}
		}
		// Re-push up after the additions
		for(int r = 0; r < size - 1; r++){
			for(int i = size - 1; i > 0; i--){
				for(int j = 0; j < size; j++){
					if(game[i][j]==0){
						game[i][j] = game[i-1][j];
						game[i-1][j] = 0;
					}
				}
			}
		}
		addTile();
	}

	//Modifies game within size constraints then adds a new tile
	void swipeLeft(){
		// Push left
		for(int r = 0; r < size - 1; r++){
			for(int j = 0; j < size - 1; j++){
				for(int i = 0; i < size; i++){
					if(game[i][j]==0){
						game[i][j] = game[i][j+1];
						game[i][j+1] = 0;
					}
				}
			}
		}
		// Do all possible additions/combinations
		for(int j = 0; j < size - 1; j++){
			for(int i = 0; i < size; i++){
				if(game[i][j+1]==game[i][j] && game[i][j] != 0){
					game[i][j] += game[i][j+1];
					if(game[i][j] == 2048){
						num2048++;
					}
					game[i][j+1] = 0;
					score += game[i][j];
					currentTiles--;
				}
			}
		}
		// Re-push up after the additions
		for(int r = 0; r < size - 1; r++){
			for(int j = 0; j < size - 1; j++){
				for(int i = 0; i < size; i++){
					if(game[i][j]==0){
						game[i][j] = game[i][j+1];
						game[i][j+1] = 0;
					}
				}
			}
		}

		addTile();

	}

	//Modifies game within size constraints then adds a new tile
	void swipeRight(){
		// Push right
		for(int r = 0; r < size - 1; r++){
			for(int j = size - 1; j > 0; j--){
				for(int i = 0; i < size; i++){
					if(game[i][j] == 0){
						game[i][j] = game[i][j-1];
						game[i][j-1] = 0;
					}
				}
			}
		}
		// Do all possible additions
		for(int j = size - 1; j > 0; j--){
			for(int i = 0; i < size; i++){
				if(game[i][j-1]==game[i][j] && game[i][j] != 0){
					game[i][j] += game[i][j];
					if(game[i][j] == 2048){
						num2048++;
					}
					game[i][j-1] = 0;
					score += game[i][j];
					currentTiles--;
				}
			}
		}
		// Re-push up after the additions
		for(int r = 0; r < size - 1; r++){
			for(int j = size - 1; j > 0; j--){
				for(int i = 0; i < size; i++){
					if(game[i][j] == 0){
						game[i][j] = game[i][j-1];
						game[i][j-1] = 0;
					}
				}
			}
		}
		addTile();
	}

	//Quick check to see if the game could be over based on if the number of tiles
	bool isCont(){
		if(currentTiles == size*size){
			return isValidMove();
		}
		else{
			return true;
		}
	}

	//If called will check every possible movement in game and see if any are valid
	//When first valid movement is found, returns true
	bool isValidMove(){
		//Check if Up is valid
		for(int i = 0; i < size - 1; i++){
			for(int j = 0; j < size; j++){
				if((game[i][j] == game[i+1][j] && game[i][j]!=0) || (game[i][j] == 0 && game[i+1][j] != 0)){
					return true;
				}
			}
		}
		//Since no return checks if Left is Valid
		for(int j = 0; j < size - 1; j++){
			for(int i = 0; i < size; i++){
				if((game[i][j]==game[i][j+1] && game[i][j]!=0) || (game[i][j] == 0 && game[i][j+1] != 0)){
					return true;
				}
			}
		}
		//Since no return checks if Down is Valid
		for(int i = size - 1; i > 0; i--){
			for(int j = 0; j < size; j++){
				if((game[i][j]==game[i-1][j] && game[i][j]!=0) || (game[i][j] == 0 && game[i-1][j] != 0)){
					return true;
				}
			}
		}
		//Since no return checks if Right is Valid
		for(int j = size - 1; j > 0; j--){
			for(int i = 0; i < size; i++){
				if((game[i][j]==game[i][j-1] && game[i][j]!=0) || (game[i][j] == 0 && game[i][j-1] != 0)){
					return true;
				}
			}
		}
		//No valid moves found
		return false;
	}

	//Before swiping, checks if the chosen swipe is a valid move
	bool isValidMove(char swipe){
		if(swipe == 'w'){
			for(int i = 0; i < size - 1; i++){
				for(int j = 0; j < size; j++){
					if((game[i][j] == game[i+1][j] && game[i][j]!=0) || (game[i][j] == 0 && game[i+1][j] != 0)){
						return true;
					}
				}
			}
		}
		else if(swipe == 'a'){
			for(int j = 0; j < size - 1; j++){
				for(int i = 0; i < size; i++){
					if((game[i][j]==game[i][j+1] && game[i][j]!=0) || (game[i][j] == 0 && game[i][j+1] != 0)){
						return true;
					}
				}
			}
		}
		else if(swipe == 's'){
			for(int i = size - 1; i > 0; i--){
				for(int j = 0; j < size; j++){
					if((game[i][j]==game[i-1][j] && game[i][j]!=0) || (game[i][j] == 0 && game[i-1][j] != 0)){
						return true;
					}
				}
			}
		}
		else if(swipe == 'd'){
			for(int j = size - 1; j > 0; j--){
				for(int i = 0; i < size; i++){
					if((game[i][j]==game[i][j-1] && game[i][j]!=0) || (game[i][j] == 0 && game[i][j-1] != 0)){
						return true;
					}
				}
			}
		}
		return false;
	}

	//save method to save current game to gameData.txt
	void save(){
		//standard c++ write file
		ofstream myfile;
		myfile.open ("gameData.txt");
		//size follow by a line break, size needs to be first for loading operations
		myfile << size << endl;
		//places only the size x size values from the 8 x 8 game
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++){
				myfile << game[i][j] << endl;
			}
		}
		//places other relevant ints
		myfile << score << endl;
		myfile << currentTiles << endl;
		myfile << num2048 << endl;
		//uses 0 and 1 to repersent an int
		if(hasWon){
			myfile << "1" << endl;
		}
		else{
			myfile << "0" << endl;
		}
		//closes and saves file
		myfile.close();
	}

	//load method to load current game from gameData.txt
	void load(){
		//value will hold file, line by line
		string value;
		//standard file reading
		ifstream myfile ("gameData.txt");

		//sets value to be current line, then converts into int
		getline( myfile, value);
		size = atoi(value.c_str());


		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++){
				//For each relevant position
				//sets value to be current line, then converts into int
				getline( myfile, value);
				game[i][j] = atoi(value.c_str());
			}
		}

		//sets value to be current line, then converts into int
		getline( myfile, value);
		score = atoi(value.c_str());

		//sets value to be current line, then converts into int
		getline( myfile, value);
		currentTiles = atoi(value.c_str());

		//sets value to be current line, then converts into int
		getline( myfile, value);
		num2048 = atoi(value.c_str());

		//sets value to be current line, then turns 0 or 1 into a bool
		getline( myfile, value);
		if(atoi(value.c_str()) == 1){
			hasWon = true;
		}
		else{
			hasWon = false;;
		}
		//always be close those files.
		myfile.close();
	}

	//Takes advantage of curses to create a pseudo GUI
	void disp(){
		//Removes everything in virtual screen
		clear();
		//Standard color choice for game (black on white bg)
		attrset(COLOR_PAIR(1));
		//Creates board and places numbers in relevant place
		printw(" ");
		for(int k = 0; k < size; k ++){
			printw("________________");
		}
		printw("\n");
		for(int k = 0; k < size; k ++){
			printw("|\t\t|");
		}
		printw("\n");

		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++){
				if(game[i][j]!=0){
					//switch to change the color of the values in the game based on value
					switch (game[i][j]){
					case 2:
						printw("|\t");
						attrset(COLOR_PAIR(1));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
						break;
					case 4:
						printw("|\t");
						attrset(COLOR_PAIR(2));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
						break;
					case 8:
						printw("|\t");
						attrset(COLOR_PAIR(3));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
						break;
					case 16:
						printw("|\t");
						attrset(COLOR_PAIR(4));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
						break;
					case 32:
						printw("|\t");
						attrset(COLOR_PAIR(5));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
						break;
					case 64:
						printw("|\t");
						attrset(COLOR_PAIR(6));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
						break;
					case 128:
						printw("|\t");
						attrset(COLOR_PAIR(7));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
						break;
					case 256:
						printw("|\t");
						attrset(COLOR_PAIR(8));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
						break;
					case 512:
						printw("|\t");
						attrset(COLOR_PAIR(9));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
						break;
					case 1024:
						printw("|\t");
						attrset(COLOR_PAIR(10));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
						break;
					default:
						printw("|\t");
						attrset(COLOR_PAIR(11));
						printw("%d", game[i][j]);
						attrset(COLOR_PAIR(1));
						printw("\t|");
					}
				}
				else{
					printw("|\t\t|");
				}
			}
			printw("\n");
			for(int k = 0; k < size; k ++){
				printw("|\t\t|");
			}
			printw("\n");
			printw(" ");
			for(int k = 0; k < size; k ++){
				printw("________________");
			}
			printw("\n");
			if(i!=size - 1){
				for(int k = 0; k < size; k ++){
					printw("|\t\t|");
				}
				printw("\n");
			}
		}
		//takes the virtual screen and displays it on physical screen
		refresh();
		//back to default color (white on black bg)
		attrset(COLOR_PAIR(0));
	}

public:
	//Initializes game based off input size and inputs 2 random tiles
	Game(int num){
		size = num;
		cont = true;
		score = 0;
		currentTiles = 0;
		num2048 = 0;
		for(int i = 0; i < size; i++){
			for(int j = 0; j < size; j++){
				game[i][j]=0;
			}
		}
		addTile();
		addTile();
	}

	//Initializes game based off gameData.txt
	Game(bool loaded){
		load();
		cont = true;
	}

	//Where it all goes down
	void play(){
		//cont will only be false if game is ended by choice or lack of moves
		while(cont){
			//prints game board
			disp();
			//checks if you have won the game from the last swipe
			if(!hasWon && num2048>0){
				hasWon = true;
				printw("You have won the game with a Score: %d \n" , score);
				printw("Press q to quit, or any other special key to proceed\n");
				refresh();
				if(getch() == 'q'){
					cont = false;
					break;
				}
			}
			//otherwise prints the score below the disp
			else{
				printw("Score: : %d \n", score);
				refresh();
			}
			//uses acceptKey() to only receive valid input
			char choice = acceptKey();
			if(choice == 'w'){
				//checks if chosen move is valid otherwise prints a brief message
				if(isValidMove('w')){
					swipeUp();
				}
				else{
					printw("Not a valid move at this time\n");
					refresh();
				}
			}
			else if(choice == 'a'){
				//checks if chosen move is valid otherwise prints a brief message
				if(isValidMove('a')){
					swipeLeft();
				}
				else{
					printw("Not a valid move at this time\n");
					refresh();
				}
			}
			else if(choice == 's'){
				//checks if chosen move is valid otherwise prints a brief message
				if(isValidMove('s')){
					swipeDown();
				}
				else{
					printw("Not a valid move at this time\n");
					refresh();
				}
			}
			else if(choice == 'd'){
				//checks if chosen move is valid otherwise prints a brief message
				if(isValidMove('d')){
					swipeRight();
				}
				else{
					printw("Not a valid move at this time\n");
					refresh();
				}
			}
			else if(choice == 'q'){
				//ends game
				cont = false;
				break;
			}
			else if(choice == 'k'){
				//saves game and ends it
				save();
				break;
			}
			cont = isCont();
		}
		disp();
		//if game is over due to q or no moves will check score against highscores
		if(!cont){
			printw("Game Over\nFinal Score: %d\n", score);
			string value;
			ifstream myfile ("gameScores.txt");
			getline( myfile, value);
			int score4 = atoi(value.c_str());
			getline( myfile, value);
			int score5 = atoi(value.c_str());
			getline( myfile, value);
			int score6 = atoi(value.c_str());
			getline( myfile, value);
			int score7 = atoi(value.c_str());
			getline( myfile, value);
			int score8 = atoi(value.c_str());
			getline( myfile, value);
			myfile.close();
			switch(size){
			case 4:
				//if current score is higher than the highscore rewrites gameScores.txt
				if(score > score4){
					printw("Highscore: %d surpassed! Congratulations, saved new Highscore: %d\n", score4, score);
					ofstream myfile;
					myfile.open ("gameScores.txt");
					myfile << score << endl;
					myfile << score5 << endl;
					myfile << score6 << endl;
					myfile << score7 << endl;
					myfile << score8 << endl;
					myfile.close();
				}
				//otherwise just ends after message
				else{
					printw("Highscore: %d not surpassed.\nBetter luck next time\n", score4);
				}
				break;
			case 5:
				if(score > score5){
					//if current score is higher than the highscore rewrites gameScores.txt
					printw("Highscore: %d surpassed! Congratulations, saving new Highscore: %d\n", score5, score);
					ofstream myfile;
					myfile.open ("gameScores.txt");
					myfile << score4 << endl;
					myfile << score << endl;
					myfile << score6 << endl;
					myfile << score7 << endl;
					myfile << score8 << endl;
					myfile.close();
				}
				//otherwise just ends after message
				else{
					printw("Highscore: %d not surpassed.\nBetter luck next time\n", score5);
				}
				break;
			case 6:
				if(score > score6){
					//if current score is higher than the highscore rewrites gameScores.txt
					printw("Highscore: %d surpassed! Congratulations, saving new Highscore: %d\n", score6, score);
					ofstream myfile;
					myfile.open ("gameScores.txt");
					myfile << score4 << endl;
					myfile << score5<< endl;
					myfile << score << endl;
					myfile << score7 << endl;
					myfile << score8 << endl;
					myfile.close();
				}
				//otherwise just ends after message
				else{
					printw("Highscore: %d not surpassed.\nBetter luck next time\n", score6);
				}
				break;
			case 7:
				if(score > score7){
					//if current score is higher than the highscore rewrites gameScores.txt
					printw("Highscore: %d surpassed! Congratulations, saving new Highscore: %d\n", score7, score);
					ofstream myfile;
					myfile.open ("gameScores.txt");
					myfile << score4 << endl;
					myfile << score5 << endl;
					myfile << score6 << endl;
					myfile << score << endl;
					myfile << score8 << endl;
					myfile.close();
				}
				//otherwise just ends after message
				else{
					printw("Highscore: %d not surpassed.\nBetter luck next time\n", score7);
				}
				break;
			case 8:
				if(score > score8){
					//if current score is higher than the highscore rewrites gameScores.txt
					printw("Highscore: %d surpassed! Congratulations, saving new Highscore: %d\n", score8, score);
					ofstream myfile;
					myfile.open ("gameScores.txt");
					myfile << score4 << endl;
					myfile << score5 << endl;
					myfile << score6 << endl;
					myfile << score7 << endl;
					myfile << score << endl;
					myfile.close();
				}
				//otherwise just ends after message
				else{
					printw("Highscore: %d not surpassed.\nBetter luck next time\n", score8);
				}
				break;
			}

		}
		//If game ended without cont being false, user chose to save
		else{
			printw("Game Saved\nCurrent Saved Score: %d\n", score);
		}
	}
};

int main(int arc, char **argv){
	cout << "1. New Game\n";
	cout << "2. Load Game\n";
	cout << "3. Highscores\n";
	cout << "4. Instructions\n";
	cout << "5. Quit\n";

	//Creates a random seed based on time
	srand (time(NULL));

	char key;
	cin >> key;
	//curses stuff
	//Creates virtual window
	initscr();
	//Removes text feedback in virtual window from getch()
	noecho();
	//Removes the requirement of a return key for getch()
	raw();
	//Adds functionality for getch() to read arrowkeys and other special characters
	keypad(stdscr, TRUE);
	//Adds functionality for curses
	start_color();

	//Creates a color pair for every number in 2048 [2,2048]
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_CYAN, COLOR_WHITE);
	init_pair(3, COLOR_GREEN, COLOR_WHITE);
	init_pair(4, COLOR_YELLOW, COLOR_WHITE);
	init_pair(5, COLOR_BLUE, COLOR_WHITE);
	init_pair(6, COLOR_RED, COLOR_WHITE);
	init_pair(7, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(8, COLOR_YELLOW, COLOR_WHITE);
	init_pair(9, COLOR_GREEN, COLOR_WHITE);
	init_pair(10, COLOR_BLUE, COLOR_WHITE);
	init_pair(11, COLOR_CYAN, COLOR_WHITE);
	//will be the color of 2048 and higher
	init_pair(12, COLOR_RED, COLOR_WHITE);

	//Selection menu
	while(true){
		//sets color to default (white on black bg)
		attrset(COLOR_PAIR(0));
		//prompts user for a dimen then creates a game(dimen) and plays it
		if(key == '1'){
			int dimen = 0;
			while(dimen < 4 || dimen > 8){
				printw("Input board size\n4. 4x4(Standard)\n5. 5x5\n6. 6x6\n7. 7x7\n8. 8x8\n");
				refresh();
				cin >> dimen;
			}

			Game* g = new Game(dimen);
			g->play();
			break;
		}
		//loads a previously saved game
		else if(key == '2'){
			Game* g = new Game(true);
			g->play();
			break;
		}
		//Prints out a list of high scores read from gameScores.txt
		else if(key == '3'){
			clear();
			printw("1. New Game\n");
			printw("2. Load Game\n");
			printw("3. Highscores\n");
			printw("4. Instructions\n");
			printw("5. Quit\n\n");
			ifstream myfile ("gameScores.txt");
			string line;

			for(int i = 4; i < 9; i++){
				getline( myfile, line);
				printw("Highest for %dx%d grid: %d\n\n",i,i,atoi(line.c_str()));
			}
			refresh();
			myfile.close();
		}
		//Prints out a quick set of instructions
		else if(key == '4'){
			clear();
			printw("1. New Game\n");
			printw("2. Load Game\n");
			printw("3. Highscores\n");
			printw("4. Instructions\n");
			printw("5. Quit\n\n");
			printw("Move tiles up and down, left and right using the arrow keys.\n");
			printw("When two equal numbers touch, they will sum up. Your goal is to reach 2048.\n");
			printw("At any point press 's' to save your game or 'q' to end without saving.\n");
			refresh();
		}
		//Exits selection menu without starting a game
		else if(key == '5'){
			clear();
			printw("1. New Game\n");
			printw("2. Load Game\n");
			printw("3. Highscores\n");
			printw("4. Instructions\n");
			printw("5. Quit\n");
			break;
		}
		cin >> key;
	}
	//Final confirmation before closing game completely
	printw("Press q to close window\n");
	refresh();
	int ch = getch();
	while(ch != 'q'){
		ch = getch();
	}
	endwin();
}
