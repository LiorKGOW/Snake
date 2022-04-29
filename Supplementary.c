#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

#define UP 72    // Up Arrow 
#define DOWN 80  // Down Arrow 
#define LEFT 75  // Left Arrow 
#define RIGHT 77 // Right Arrow 
#define ESC 27   // Escape Key

void gotoxy(int y, int x);
int getKey();

void main()
{
	// Testing the functions of supplementary:

	char c;
	int x = 40, y = 12;  // x for coulmn and y for line 

	// start from the middle of the screen
	gotoxy(x, y);
	// the direction is Right untill an arrow hit
	c = RIGHT;

	while (c != ESC)
	{
		// sleeps for half a second before continuing to the next command
		Sleep(500);

		// only if a key was hit we read what key code it was
		if (_kbhit()) // if any key was hit
			c = getKey();  // change direction
		// make a move in current direction if no change
		switch (c)
		{
		case RIGHT:
			x++;
			break;
		case DOWN:
			y++;
			break;
		case LEFT:
			x--;
			break;
		case UP:
			y--;
			break;
		}

		gotoxy(x, y);
		printf("*");
		gotoxy(26, 81);
	}
}

/*************************************************************************************************************************
* This function moves the cursor to the line y and column x.
* (0,0) is the upper left corner of the screen.
*/
void gotoxy(int y, int x) {
	printf("\x1b[%d;%df", x + 1, y + 1);
}

/*************************************************************************************************************************
* This function gets keyCodes for special keys such as arrows , escape and others
* It returns the Key code such as 72 for arrow up, 27 for Escape... 
*/
int getKey()
{
	char KeyStroke = _getch();
	if (KeyStroke == 0 || KeyStroke == -32)
		KeyStroke = _getch();
	return (KeyStroke);
}

/*************************************************************************************************************************/
