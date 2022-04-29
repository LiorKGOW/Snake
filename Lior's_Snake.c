
/**********************************************************************
   ID: 208547992

   Name: Lior Keren

/*************************************************************************

   The program is a game called "Snake" (Version 1.1).

   New In This Update:
   First of all, The Program prints the Main Menu of the game, in addition to an explanation of the rules of the game.

   After the Player has chosen a difficulty for the current game, the board is displayed on the screen for the Player to see.
   (Including with the Starting Food, and the Snake which is facing to the Right! :) ).
   At this stage of the game, the Program expects an Input from the Player.
   Once the Player hits a key (UP / DOWN / RIGHT / LEFT / ESC),
   The game starts and the Snake starts to move constantly in the direction that it has been instructed to by the Player (In case of an arrow).
   In case of ESC, The
   For any other key, the Program does nothing.

   The Snake will move in the same direction that was instructed by the Player, Until a new arrow had been hit (with a different direction).
   If the Snake eats the Food (which is indicated with '$'), It grows by 1 in Size, and a new piece of Food appears on the Screen.

   The Goal of the game is to "eat" 10 pieces of Food. When the Player succeeds, an appropriate message would be printed on the screen.
   If the Player (the Snake) had hit itself, or had hit a wall,
   Or the Player had hit the ESC button on the keyboard, The game will instantly end with a defeat (The Player had Lost).
   (And an appropriate message will be printed on the screen for the Player).

   When a game ends, after the approriate message had been printed, the Program asks the Player whether or not he wants to play again.
   If the Player inserted 'y', the Main Menu will appear again, and the Player can play the game again.
   If the Player inserted 'n', the Program will End.
   Good Luck !!

   ASSUMPTIONS: The Player inserts 'y' or 'n' at the end, and a number in the choices at the Main Menu.

 *********************************************************************/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <windows.h>

#define MAXSIZE 20   // Max Size of the Snake.
#define STARTINGSIZE 10   // The Starting Size of the Snake.
#define ENDSIZE 20   // The End Size of the Snake. (if the Player has reached this size, the Player has Won the game).
 // Must be <= MAX SIZE !!!

#define BOARDSIZER 25   // Board ROW Size (HAS TO BE BIGGER THAN 20)
#define BOARDSIZEC 75   // Board COL Size (HAS TO BE BIGGER THAN 20)

#define ENDROW BOARDSIZER + 1   // The position of the Wall on the right side of the board. 0 is the FIRST row (the wall on the left side of the board).
#define ENDCOL BOARDSIZEC + 1   // The position of the Wall on the lower side of the board. 0 is the FIRST col (the wall on the upper side of the board).

#define HIDEROW BOARDSIZER + 10  // Hide Cordinates for the cursor according to the Size of the Board (ROW)
#define HIDECOL 0                // (& COL)

#define UP 72     // Up Arrow 
#define DOWN 80   // Down Arrow 
#define LEFT 75   // Left Arrow 
#define RIGHT 77  // Right Arrow 
#define ESC 27    // Escape Key

#define SPEED 200   // The Speed of the Snake (1 step in 1 second).
					// If you wish to change it to a higher speed, reduce this number.
                    // different Speeds:
#define SPD1 1000   // 1. 1000 Very Slow
#define SPD2 500    // 2. 500 Slow
#define SPD3 300    // 3. 300 Normal
#define SPD4 200    // 4. 200 Fast
#define SPD5 100    // 5. 100 Very Fast
#define SPD6 50     // 6. 50 Insanely Fast (Good Luck)

typedef struct Rowcol {
	int row;
	int col;
}Rowcol;

// Explanation about the Functions used at the head of each one (after main).

Rowcol StartGame(Rowcol[], int, int* SPD);
int PrintMenu(int*);
Rowcol PrintBoard(Rowcol[], int);

bool AreEqual(Rowcol, Rowcol);
void MoveSnake(Rowcol[], int*, int, Rowcol);
Rowcol AddFood(Rowcol[], int);

bool PrintMessage(bool);
void GameReset(Rowcol*, int*, Rowcol*, char*, char*, bool*, bool*, int*);

bool SnakeHit(Rowcol[], int, Rowcol);

//  Supplemantery Functions:
void gotoxy(int, int);
int getKey();

/*************************************************************************************************************************/
void main() {

	Rowcol S[MAXSIZE];         // Array of Rowcol's (The Snake). 
							   // Every piece of the Snake is indicated with a Location on the board.
	int size = STARTINGSIZE;   // The CURRENT Size of the Snake (always: size < MAXSIZE).
							   // Size is initialized to the Starting Size of the Snake.

	Rowcol Food;        // The Location of the CURRENT Food in the game.

	char Key = 'a';     // Are used to determine in which direction the snake will go (ESC to stop the game and Exit back to Main Menu).
	char Temp = 'a';    // Helps the Program determine whether or not or not the input was correct. Can be only RIGHT / LEFT / UP / DOWN / ESC ! (Except for the start of the game).

	Rowcol newloc;      // The new Location that the Snake will be going to next.

	bool EndGame = false;      // End the current Game, The Player had LOST.
	bool Win = false;          // End the current Game, The Player had WON.
	bool PlayAgain = false;    // The Player wants to play again.

	int SPD = SPEED;

	Food = StartGame(S, STARTINGSIZE, &SPD);   // Starts a Game, returns the Location of the first Food / 0 in Food.row to Exit


	// while the Player hasn't chosen to Exit:
	while (Food.row != 0)
	{
		// while the Player hasn't hit ESC, or the game hasn't ended yet:
		while (Key != ESC && !EndGame && !Win)
		{
			// Hide The Cursor before sleep:
			gotoxy(HIDEROW, HIDECOL);

			// Sleeps for half a second before continuing to the next command
			Sleep(SPD);

			// Only if a key was hit we read what key code it was.
			if (_kbhit()) // if any key was hit
				Key = getKey();  // change direction

			// If not changed, make a move in the current direction. 
			// Else, change the direction.

			/* Check if Key is NOT a correct Input:
			*  If it is NOT, change it back to the last correct Input (which is kept in Temp).
			*  If it is a correct input, keep it in temp. 
			*/

			if (!(Key == RIGHT || Key == LEFT || Key == UP || Key == DOWN || Key == ESC))
				Key = Temp;
			else
				Temp = Key;

			// First, set the New Location to the position of the Head of the Snake (S[0]).
			newloc = S[0];

			// Then, adjust the New Location, according to the Key that the Player had chosen:
			switch (Key)
			{
			case RIGHT:   // Go RIGHT
				newloc.col++;
				break;
			case DOWN:    // Go DOWN
				newloc.row++;
				break;
			case LEFT:    // Go LEFT
				newloc.col--;
				break;
			case UP:      // Go UP
				newloc.row--;
				break;
			default:      // For any other key, do nothing (There won't be any other key, thanks to the check with temp ! At the start of the loop)
				break;
			}

			if (Key != 'a') {   // If Key == 'a' -> The game has NOT started yet! So do nothing.
								// Only if Key != 'a', Start playing (Key will be only RIGHT / LEFT / UP / DOWN in this case! thanks to the help of Temp).

				// First Check if the Player had LOST:

				// Check if the new location is on a piece of the snake
				EndGame = SnakeHit(S, size, newloc);
				// EndGame = AreEqual(newloc, S[1]);  

				if (!EndGame)   // I the last check passed (EndGame == false), check if the new Location is one of the walls of the board.
				{
					if (newloc.row == 0 || newloc.row == ENDROW || newloc.col == 0 || newloc.col == ENDCOL)
						EndGame = true;
				}

				// The Player hasn't Lost -> Continue Playing :

				if (!EndGame && AreEqual(newloc, Food))  // The Snake is going to eat the Food
				{
					// Move the Snake, add 1 and Print the new State of the game on the screen for the Player. 
					// In this case, newLoc == Food. So MoveSnake will "delete" from the screen the current Food!
					MoveSnake(S, &size, size + 1, newloc);

					// Check if the Snake has reached the Winning Size
					if (size == ENDSIZE) {
						Win = true;    // End the Game, the Player has Won!
					}
					else   // The game hasn't ended yet. So we need a NEW piece of Food.
					{
						Food = AddFood(S, size);
					}
				}
				if (Win)   // If the Player Has WON
					PlayAgain = PrintMessage(Win);

				else {    // If the Player Hasn't won:

					if (EndGame)   // The Snake had hit itself or a wall -> The Player has LOST!
						PlayAgain = PrintMessage(false);

					else   // EndGame == false. If the game hasn't ended, and the Snake just needs to go forward, Move it
						MoveSnake(S, &size, size, newloc);
				}
			}
		}
		if (Win)   // If the Player Has WON -> WIN
			PlayAgain = PrintMessage(Win);     // Ask the Player if he wants to play again

		else      // The Player has hit the ESC key, or he had LOST -> LOSE
			PlayAgain = PrintMessage(false);   // Ask the Player if he wants to play again

		if (PlayAgain) {   // If the Player wants to play again, Reset the game to the initial state, and start a new game.

			GameReset(S, &size, &Food, &Key, &Temp, &Win, &EndGame, &SPD);
		}
		else   // The Player wants to EXIT
		{
			Food.row = 0;   // To End the Main loop
		}
	}
	// When the loop is over, Exit the game:
	system("cls");
	gotoxy(0, 0);
	printf("Thank you for playing :) \nSee you next time! \n");
}

/*************************************************************************************************************************
* StartGame sets up the game.It gets the empty Snake(S) and fills it with the starting positions.
* In addition, the function gets the starting Size of the Snake(size).
*
* (**In this Update, there is only the BASIC level of the game,
*So if the player has chosen 2 / 3, the function would ask for a different choiceand would print the Menu again.**)
*
* If he chose 1 - 3, it arranges the gameand sets up the board of the game.
*
* Startgame returns the Location of the first Food(Food).
* If the player has chosen to Exit the game, the function returns 0 in the Row & the Col of the returned value.
*/
Rowcol StartGame(Rowcol S[], int size, int* SPD) {

	int Choice;    // The Choice of the Player in the Main Menu.
	Rowcol Food;   // The Location of the current Food on the screen.
	Rowcol Start = { BOARDSIZER / 2 , BOARDSIZEC / 2 };   // The Middle of the screen +-

	Choice = PrintMenu(SPD);   // Prints the Main Menu, and asks for a Choice from the Player.
							// Choice is 0 / 1 (In this Update!).

	if (Choice != 0)
	{
		// Setting the Snake on the board:

		for (int i = 0; i < STARTINGSIZE; i++)
		{
			S[i].row = Start.row;
			S[i].col = Start.col - i;
		}

		Food = PrintBoard(S, size);
	}
	else  // Choice == 0 -> The Player wants to Exit the game.
	{
		// Set Food as row = 0, col = 0: 
		// (in order to End the while loop in Main)

		Food.row = 0;
		Food.col = 0;
	}

	return Food;
}

/*************************************************************************************************************************
* PrintMenu prints the Main Menu of the game.
* The function doesn't recieve anything.
* PrintMenu returns, at the end, the Choice of the player for the level of difficulty of the game(1 - 3),
* (**In this Update, there is only the BASIC level of the game,
*So if the player has chosen 2 / 3, the function would ask for a different choiceand would print the Menu again.**)
* Or 0 if the player wants to Exit the game.
* If the player entered a different Choice, The function prints the Menu againand ask for a different Choice.
*
* ASSUMPTION: The function assumes that the Player is going to hit a number!
*/
int PrintMenu(int* SPD) {

	int Choice = -1;   // The Choice of the Player

	system("cls");

	while (!(Choice >= 0 && Choice <= 6))
	{
		printf("******************************************************************************* \n");
		printf("* Welcome to Lior's Snake Version 1.1!                                        * \n");
		printf("* Choose Your Difficulty:                                                     * \n");
		printf("* 0 - Exit the Game                                                           * \n");
		printf("* 1 - Old                                                                     * \n");
		printf("* 2 - Beginner                                                                * \n");
		printf("* 3 - Novice                                                                  * \n");
		printf("* 4 - Normal                                                                  * \n");
		printf("* 5 - Skilled                                                                 * \n");
		printf("* 6 - Insane (Good Luck)                                                      * \n");
		printf("*                                                                             * \n");
		printf("* Rules of the Game:                                                          * \n");
		printf("* 1. The Snake is constantly moving in the direction that it is instructed to * \n");
		printf("* 2. In order to change the direction of the Snake, hit one of the arrows     * \n");
		printf("* 3. You Need to Eat 10 pieces of Food ($)                                    * \n");
		printf("* 4. You must NOT hit yourself, or a wall !                                   * \n");
		printf("* Good Luck !! :)                                                             * \n");
		printf("******************************************************************************* \n");

		// Read ASSUMPTION!
		scanf("%i", &Choice);

		system("cls");

		if (!(Choice >= 0 && Choice <= 6))
			printf("Please enter a valid choice! (a number between 0 and 1) \n\n");
	}

	switch (Choice) {
	case 1:
		*SPD = SPD1;
		break;
	case 2:
		*SPD = SPD2;
		break;
	case 3:
		*SPD = SPD3;
		break;
	case 4:
		*SPD = SPD4;
		break;
	case 5:
		*SPD = SPD5;
		break;
	case 6:
		*SPD = SPD6;
		break;
	default:
		break;
	}

	return Choice;
}

/*************************************************************************************************************************
* PrintBoard recieves the Snake(S), and it's current Size (size).
* The function prints the board on the screen for the Player to see.
* Including the Snake itself, and the First Food of the game(with the help of AddFood).
* The Snake is facing to the right!:) (@@@@@@@@@ > ).
*
* PrintBoard returns, by value, the Location of the First Food on the board(Location).
*/
Rowcol PrintBoard(Rowcol S[], int size) {

	Rowcol Location;   // Gets a Random Location on the board (which at the end of the function will be the Location of the First Food).

	// Clears the screen:
	system("cls");

	for (int i = 0; i < BOARDSIZER + 2; i++)
	{
		if (i == 0 || i == BOARDSIZER + 1)   // FIRST row or LAST row
		{
			for (int j = 0; j < BOARDSIZEC + 2; j++)
				printf("#");
			printf("\n");
		}
		else
		{
			for (int j = 0; j < BOARDSIZEC + 2; j++)
			{
				if (j == 0 || j == BOARDSIZEC + 1)   // Only for the FIRST col in the row or the LAST col in the row.
					printf("#");
				else
					gotoxy(i, BOARDSIZEC + 1);   // Only once for each row!, after this command, at the end of the iteration of the loop, j == BOARDSIZEC + 1 !
			}
			printf("\n");
		}
	}

	// Add a new piece of Food to the Game:

	Location = AddFood(S, STARTINGSIZE);   // Location is now the Location of the FIRST Food.

	// Print the Snake on the board
	for (int i = 0; i < STARTINGSIZE; i++) {
		gotoxy(S[i].row, S[i].col);
		printf("%c", '@');
	}

	return Location;
}

/*************************************************************************************************************************
* AreEqual recieves to structs of location(rc1& rc2).
* The function checks if rc1and rc2 are the same location on the board.
* AreEqual returns true if rc1and rc2 have the same values(They are the same location),
* And returns false if not.
*/
bool AreEqual(Rowcol rc1, Rowcol rc2) {

	return (rc1.row == rc2.row) && (rc1.col == rc2.col);
}

/*************************************************************************************************************************
* MoveSnake recieves the Snake (S), the current Size of the Snake by reference (size), the new Size of the Snake (newSize) and the new location of the head of the Snake.
*
* ASSUMPTIONS: MoveSnake assumes that the new Location is INSIDE the playing area! (is NOT out of bounds),
* Another assumpsion is that Snake has space to contain the new piece, if it needs to be added to Snake.
*
* First, MoveSnake will move every piece forward in the Snake array (each piece will be in the location of the one before it).
* Then, The function checks if the newSize is equal to the original Size of the Snake.
* if newSize is bigger then size, the function adds another piece to the Snake which will be it's original Tail (the last piece of the Snake)
* else, the function deletes the previous Tail of the Snake from the screen which was the location of the ORIGINAL last piece of the Snake (because it moved).
* Lastly, the function prints the new head of the Snake in the new location it was given (newloc).
*
* MoveSnake doesn't return anything, Just prints (the original Snake will be changed, and if a piece had been added to Snake, the origianl size would be changed too).
*/

void MoveSnake(Rowcol S[], int* size, int newSize, Rowcol newloc) {

	Rowcol temp1;   // temp1 & temp2 Are used in the last section of the function.
	Rowcol temp2;   // They help move the pieces in the Snake accordingly (and to add a new one if needed to).
	bool AteFood = false;   // Weird but it doesn't work

	// Moving every piece of the Snake forward:
	for (int i = 0; i < (*size) - 1; i++)
	{
		if (i == 0) {
			temp1 = S[0];
			S[0] = newloc;
		}
		else {  // i > 0
			temp2 = S[i];
			S[i] = temp1;
			temp1 = temp2;
		}
	}

	// Save the value of the Tail in temp1
	temp1 = S[(*size) - 1];

	// Set Tail as the one before it
	S[(*size) - 1] = temp2;

	if (newSize == *size)   // we just need to "delete" the original Tail from the screen
	{
		gotoxy(temp1.row, temp1.col);
		printf(" ");
	}
	else  // newSize > size -> we don't need to "delete" the original Tailform the screen, because it is the NEW Tail.
		  // In this case, we need to add to Snake another piece, which will be the previous Tail (kept in temp1).
	{
		S[*size] = temp1;
		(*size)++;   // add 1 to the original size of the Snake, because a new piece has just been added.
		AteFood = true;
	}

	// Print the new Head:
	gotoxy(newloc.row, newloc.col);

	if (AteFood)  // Yes
		printf("Q");
	else          // No
		printf("@");
}

/*************************************************************************************************************************
* AddFood recieves the Snake (S), and the current Size of it (size).
* The function prints on the screen a new Food (which is indicated with: '$').
* The function makes sure that the new Food will be in the bounds of the game, and that ISN'T on the position of one of the pieces of the Snake.
* AddFood returns the Location of the new Food that is visible on the screen at the end of it.
* (In addition to printing the new Food on the screen for the player to see).
*/
Rowcol AddFood(Rowcol S[], int size) {

	Rowcol Location;   // Gets a random Location on the board (which at the end of the function will be the Location of the New Food).
	bool IsSnakeLocation = false;   // Is used in order to determine wheather or not the location of the random Food
									// is the location of one of the pieces of the snake. (Is used in the while loop)

	//  move Cursor to a random place for the food:
	srand(time(NULL));

	// Random Location on the board that ISN'T the snake's Location:

	// Give a random location that is NOT out of bounds
	Location.row = (rand() % BOARDSIZER) + 1;   // a random number between 1 - BOARDSIZER + 1 
	Location.col = (rand() % BOARDSIZEC) + 1;   // a random number between 1 - BOARDSIZEC + 1

	// check if the Location is the location of one of the pieces of the snake:
	for (int i = 0; i < size && !IsSnakeLocation; i++) {
		IsSnakeLocation = IsSnakeLocation || (Location.row == S[i].row && Location.col == S[i].col);
	}

	while (IsSnakeLocation)  // While the Location of the Food is in one of the "pieces" of the snake, change it
	{
		Location.row = (rand() % BOARDSIZER) + 1;   // a random number between 1 - BOARDSIZER + 1
		Location.col = (rand() % BOARDSIZEC) + 1;   // a random number between 1 - BOARDSIZEC + 1

		for (int i = 0; i < size && !IsSnakeLocation; i++) {
			IsSnakeLocation = IsSnakeLocation || (Location.row == S[i].row && Location.col == S[i].col);
		}
	}
	// The Location of the Food is good now (NOT the Location of the Snake or out of bounds).

	//  Print the Food:
	gotoxy(Location.row, Location.col);
	printf("$");

	return Location;
}

/*************************************************************************************************************************
* PrintMessage recieves a bool variable named IsWin.
* if IsWin is true, The function Prints a Winning Message for the Player in the appropriate Location(The Player Had WON : ) ).
* else, IsWin is false, The function prints a Losing Message for the Player in the appropriate Location(The Player Had Lost : ().
* After Printing the EndGame Message, PrintMessage asks the Player weather or not he wants to play the game again.
* PrintMessage returns wheather or not the Player wants to play again(true for yes / false for no).
*
* ASSUMPTION : The function assumes the Player had inserted y / n ONLY.
*/
bool PrintMessage(bool IsWin) {

	char Choice;   // The Choice of the Player whether or not he wants to play again.

	// Clear the screen:
	system("cls");

	// Print Win / Lose Message accordingly in a "fancy" way:
	for (int i = 0; i < 50; i++) {
		printf("$");
	}

	printf("\n$                                                $");

	if (IsWin)
		printf("\n$  Well Done! You have Won! Congratiolations :)  $\n");
	else
		printf("\n$  Too Bad.. You Lost :( Better Luck Next Time!  $\n");

	printf("$                                                $\n");

	for (int i = 0; i < 50; i++) {
		printf("$");
	}
	printf("\n\nDo You want to play again? (y/n, y is for yes, n is for no) \n");

	// Read ASSUMPTION
	scanf("%c", &Choice);

	if (Choice == 'y')
		return true;
	else   // Choice == 'n'
		return false;
}

/*************************************************************************************************************************
* GameReset recieves by reference the Snake(S), the size of it(size), the current key that the player had hit(Key& Temp that helps him), Winand EndGame.
* The function Resets ALL of them to the initial state of the game, before it started.
* GameReset Doesn't return anything (It changes the original variables to their initial states).
* If the Player had chosen to Exit the Game, GameReset returns(by reference) in* Food 0 in Food.row->to End the while loop in Main.
*/
void GameReset(Rowcol* S, int* size, Rowcol* Food, char* Key, char* Temp, bool* Win, bool* EndGame, int* SPD) {

	// First, Clear the Screen:
	system("cls");

	// Initialize Win and EndGame to false:
	*Win = *EndGame = false;

	// Initialize Key and Temp to 'a':
	*Key = *Temp = 'a';

	// Empty Every spot in Snake:
	for (int i = 0; i < MAXSIZE; i++) {
		S[i].row = 0;
		S[i].col = 0;
	}

	// Initialize Size to The Starting Size:
	*size = STARTINGSIZE;

	// Starts a new Game, returns the Location of the first Food / 0 in Food.row to Exit:
	*Food = StartGame(S, STARTINGSIZE, SPD);

	// The Game is now Reset (and Ready to Play / to Exit).
}

/*************************************************************************************************************************
* HitSnake recieves the Snake(S), the Size of it(size), and the New Location that the Snake is going to(newLoc).
* The function checks if the new Location is equal to one of the pieces of the Snake. (if that is the case, the Snake is going to hit his body)
* In this case, SnakeHit returns TRUE.
* If the new Location is different from every piece of the Snake, then the Snake is NOT going to hit his body.
* In this case, SnakeHit returns FALSE.
* ASSUMPTION - newLoc != S[0]
* (There is no possibility that S[0] will be the new Location because if the Player hasn't hit a key yet, Key == 'a' -> Main won't call this function!thanks to : if (Key != a) { game })
*/
bool SnakeHit(Rowcol S[], int size, Rowcol newLoc) {

	bool Check = false;

	for (int i = 0; i < size - 1 && !Check; i++)
		Check = Check || AreEqual(newLoc, S[i]);

	return Check;
}

/*************************************************************************************************************************
* This function moves the cursor to the line x and column y.
*  (0,0) is the upper left corner of the screen.
*  I have changed the position between x and y for convenience reasons only (Hope that is OK :) )
*/
void gotoxy(int x, int y) {
	

	printf("\x1b[%d;%df", x + 1, y + 1);
}

/*************************************************************************************************************************
* This function gets keyCodes for special keys such as arrows, escapeand others
* It returns the Key code such as 72 for arrow up, 27 for Escape...
*/
int getKey() {

	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}

/*************************************************************************************************************************/
