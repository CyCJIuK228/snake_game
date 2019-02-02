// IDEA and ALGORITHM
//
// Each Map icon represents itself by code
// -1 is a border code
// 0 is empty icon
// Fruit codes placed into Fruit array: {42,36,37,38,64,81 }/There are askii codes that represents fruits
// Snake head code is equal to  snake_body_size. If snake_body_size==1, snake head code is 1
//												 If snake_body_size==4, snake head code is 4
//
// Each part of snake exists exactly so much that snake_body_size equals
// In array that snake -> ooooo0 will be represented as [1][2][3][4][5][6]. Head is equal to snake_body_size.
// Next move makes to disappear icon with 1-code (snake`s tail)
// So we decrement each part of snake and get:[0][1][2][3][4][5]/ 0-code icon will be drawn as empty icon
// Where is snake head? if we had this snake ooo0, and user moved down, we will have thіs changes in array:
// 
//	from: [0][1][2][3][4][0]	->		to:	[0][0][1][2][3][0]			we just give current snake_body_size to icon
//	      [0][0][0][0][0][0]	->			[0][0][0][0][4][0]			that was down from previous snake head position

#include "pch.h"
#include <iostream>
#include <conio.h>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <windows.h>
#include <stdio.h>

using namespace std;

int map_width = 20, map_height = 20;
int snake_head_x = (map_width / 2) + 1;
int snake_head_y = (map_height / 2) + 1;							   //setting snake in centre of map
int snakebody_size = 1;

int fruit_x, fruit_y;

int Map[24][24];
int Fruit[] = { 42,36,37,38,64,81 };								   //askii-codes of fruits symbols

bool game_is_over = false;

enum eDirection
{
	stop = 0, up = 1, right = 2, down = 3, left = 4
};

eDirection direction = eDirection::stop;
eDirection previous_direction = eDirection::stop;


																							//Colors: 
void set_color(int ForgC)																	// 
{																							//set_color(0);  black
	WORD wColor;																			//set_color(1);  blue
	//This handle is needed to get the current background attribute							//set_color(2);  green
																							//set_color(3);  light-blue
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);										//set_color(4);  red
	CONSOLE_SCREEN_BUFFER_INFO csbi;														//set_color(5);  pink
	//csbi is used for wAttributes word														//set_color(6);  yellow
																							//set_color(7);  white
	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))
	{
		//To mask out all but the background attribute, and to add the color
		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
		SetConsoleTextAttribute(hStdOut, wColor);
	}
	return;
}

void set_random_fruit_position()
{
	do
	{
		fruit_x = rand() % 19 + 1;
		fruit_y = rand() % 19 + 1;
	} while (Map[fruit_y][fruit_x] != 0);						 //finding empty icon in map to set fruit
}

BOOL show_console_cursor(BOOL bShow)
{
	CONSOLE_CURSOR_INFO cci;
	HANDLE hStdOut;
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE)
		return FALSE;
	if (!GetConsoleCursorInfo(hStdOut, &cci))
		return FALSE;
	cci.bVisible = bShow;
	if (!SetConsoleCursorInfo(hStdOut, &cci))
		return FALSE;
	return TRUE;
}

void set_cursor_position(int x, int y)					//set cursor position in console
{														
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void map_setup()											//initialization of primary values
{
	for (int i = 0; i < map_width + 1; i++)					// borders inialization
	{
		Map[0][i] = -1;
		Map[map_height][i] = -1;
		Map[i][0] = -1;
		Map[i][map_height] = -1;
	}
	Map[snake_head_y][snake_head_x] = snakebody_size;
	set_random_fruit_position();
	Map[fruit_y][fruit_x] = Fruit[rand() % 6];
}

void draw_map()
{
	set_cursor_position(0, 0);
	for (int i = 0; i < map_width + 1; i++)
	{

		for (int j = 0; j < map_height + 1; j++)
		{

			if (Map[i][j] == -1)						//-1 is code of borders
			{
				set_color(3);
				cout << char(178);
			}
			if (Map[i][j] == 0)							//0 is code of empty icon
			{
				cout << ' ';

			}
			if ((Map[i][j] == snakebody_size))			//code of snake head that equls to snakebody_size
			{
				set_color(6);
				cout << '0';

			}
			if ((i == fruit_y) && (j == fruit_x))
			{
				set_color(rand() % 5 + 7);
				cout << char(abs(Map[fruit_y][fruit_x]));

			}
			if ((Map[i][j] < snakebody_size) && (Map[i][j] > 0)) // codes of snake tail
			{
				set_color(6);
				cout << 'o';

			}

		}
		cout << endl;
	}

	// drawing  rules and control-keys
	set_color(8);
	set_cursor_position(30, 1);
	cout << "Your Snake is located in centre of map";
	set_cursor_position(30, 2);
	cout << "Speed will be increasing during the game";
	set_cursor_position(30, 5);
	cout << "Press"; set_color(4); cout << " W "; set_color(8); cout << "to move Up";
	set_cursor_position(30, 6);
	cout << "Press"; set_color(4); cout << " A "; set_color(8); cout << "to move Left";
	set_cursor_position(30, 7);
	cout << "Press"; set_color(4); cout << " S "; set_color(8); cout << "to move Down";
	set_cursor_position(30, 8);
	cout << "Press"; set_color(4); cout << " D "; set_color(8); cout << "to move Right";
	set_cursor_position(30, 10);
	cout << "Press"; set_color(4); cout << " X "; set_color(8); cout << "twice to Exit Game";


	//drawing score
	set_color(6);
	set_cursor_position(30, 12);
	cout << "Score "; set_color(8); cout << " = "; set_color(6); cout << (snakebody_size - 1) * 10;
	set_cursor_position(0, 25);
}

void refresh_map_values()
{
	for (int i = 1; i < map_width; i++)
	{
		for (int j = 1; j < map_height; j++)
		{
			if ((Map[i][j] > 0) && !((i == fruit_y) && (j == fruit_x)))
			{
				Map[i][j]--;
			}
		}
	}
}

void check_inaccessible_directions()	//this function not allows to move right and then left, etc.
{
	if (previous_direction == up && direction == down)
	{
		direction = up;
	}
	if (previous_direction == down && direction == up)
	{
		direction = down;
	}
	if ((previous_direction == eDirection::left) && (direction == eDirection::right))
	{
		direction = eDirection::left;
	}
	if ((previous_direction == eDirection::right) && (direction == eDirection::left))
	{
		direction = eDirection::right;
	}
	previous_direction = direction;
}


void input()
{
	switch (_getch())
	{
		case 'w':
		{
			direction = eDirection::up;
			break;
		}
		case 'd':
		{
			direction = eDirection::right;
			break;
		}
		case 's':
		{
			direction = eDirection::down;
			break;
		}
		case 'a':
		{
			direction = eDirection::left;
			break;
		}
		case 'x':
		{
			game_is_over = true;
			system("cls");
			set_color(0);
			exit(0);
			break;
		}
	}

	check_inaccessible_directions();
	
}

void logic()
{
	
	switch (direction)
	{

		case eDirection::stop:
		{
			return; //exit function to get input
		}
		case eDirection::up:
		{
			snake_head_y--;
			break;
		}
		case eDirection::right:
		{
			snake_head_x++;
			break;
		}
		case eDirection::down:
		{
			snake_head_y++;
			break;
		}
		case eDirection::left:
		{
			snake_head_x--;
			break;
		}

	}

	//this checks:
	//if snake moved to not emty position (not 0 in map) and not to  fruit - it means that snake hit border
	//																					or ate own tail
	if ((Map[snake_head_y][snake_head_x] != 0) && (Map[snake_head_y][snake_head_x] != Map[fruit_y][fruit_x]))
	{
		game_is_over = true;
	}
	
	if ((snake_head_x == fruit_x) && (snake_head_y == fruit_y))
	{
		snakebody_size++;
		set_random_fruit_position();
		Map[fruit_y][fruit_x] = Fruit[rand() % 6]*(-1);
		Beep(300, 250);
	}
	else
	{
		refresh_map_values();	//decreasing tail values
	}

	Map[snake_head_y][snake_head_x] = snakebody_size;

	draw_map();

	Sleep(max(750 - (snakebody_size * 20), 250));
}

void restart_game()
{
	for (int i = 0; i < map_width + 1; i++)
	{
		Map[0][i] = -1;
		Map[map_height][i] = -1;
		Map[i][0] = -1;
		Map[i][map_height] = -1;
	}
	snake_head_x = map_width / 2 + 1, snake_head_y = map_height / 2 + 1;
	direction = eDirection::stop;
	previous_direction = eDirection::up;
	snakebody_size = 1;
	game_is_over = false;
	for (int i = 1; i < map_height; i++)
	{
		for (int j = 1; j < map_width; j++)
		{
			Map[i][j] = 0;
		}
	}
	Map[snake_head_y][snake_head_x] = snakebody_size;
	set_random_fruit_position();
	Map[fruit_y][fruit_x] = Fruit[rand() % 6];
	set_cursor_position(0, 0);

}

void game_over_scene()
{
	system("cls");
	set_cursor_position(50, 10);
	cout << "Game is Over";
	set_cursor_position(45, 12);
	set_color(8);
	cout << "Press"; set_color(4); cout << " R "; set_color(8); cout << "to Restart Game";
	set_cursor_position(45, 13);
	cout << "Press"; set_color(4); cout << " X "; set_color(8); cout << "twice to Exit Game";
	Beep(523.25, 750);
	Beep(493.88, 800);
	Beep(466.16, 1000);
}

void choice_request()
{
	switch (_getch())
	{
		
		case 'r':
		{
			restart_game();
			system("cls");
			break;
		}
	
		case 'x':
		{
			set_color(0);
			exit(0);
			break;
		}
		default:
		{
			choice_request();
		}
	}
}

int blocker = 0;				//created to block calling function twice, just a cratch, sorry (:

int main()
{
	srand(time(NULL));
	show_console_cursor(FALSE);
	map_setup();

	while (true)
	{
		blocker = 0;
		draw_map();

		while (!(game_is_over))
		{
			input();

			do
			{
				if (!(blocker % 2))
					logic();

				blocker++;
			} while (!(_kbhit()) && !(game_is_over));

		}
		game_over_scene();
			
		choice_request();
	
		Sleep(500);
	}
}

