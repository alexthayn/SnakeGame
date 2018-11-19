#include "OpenGL.h"
#include "GL/glut.h"
#include "GL/GL.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <ctime>
#include <Windows.h>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;
const int gridHeight = 50;
const int gridWidth = 60;
const int INITIAL_SIZE = 5;
const double PGenerateFruit = .5;

bool bw = false;
bool bw2 = false;
bool pauseGame = false;
int headX =20;
int headY =30 ;
int head2X = 40;
int head2Y = 30;
int grow1 = INITIAL_SIZE;
int grow2 = INITIAL_SIZE;

list<int> snakeX;
list<int> snakeY;

list<int> snake2X;
list<int> snake2Y;

char board[gridWidth][gridHeight];
char direction = 'N';
char direction2 = 'S';
bool gameOver = true;

void DrawElement(double i, double j, char element) {
	double x = (i+.5) / gridWidth * 2.0 - 1.0;
	double y = (j+.5) / gridHeight * 2.0 - 1.0;
	double r = 1.0 / gridHeight;
	if (pauseGame == true) {
		return;
	}
	switch (element) {
	case 'W':
		glColor3f(0, 0, 0);
		DrawRectangle(x-r, y-r,x+r,y+r);
		break;
	case 'S':
		glColor3f(0, bw*.5, !bw*.5);
		DrawCircle(x, y, r, 10);
		break;
	case 's':
		glColor3f(0, !bw*.5, bw*.5);
		DrawCircle(x, y, r, 10);
		break;
	case 'T':
		glColor3f(1,bw2*.5, !bw2*.5);
		DrawCircle(x, y, r, 10);
		break;
	case 't':
		glColor3f(1,!bw2*.5, bw2*.5);
		DrawCircle(x, y, r, 10);
		break;
	case 'P':
		glColor3f(0, 1, 1);
		DrawCircle(x, y, r, 10);
		break;
	case 'p':
		glColor3f(0, 1, 1);
		DrawCircle(x, y, r, 10);
		break;
	case 'A':
		glColor3f(1.0, 0.0, 0);
		DrawCircle(x, y, r, 10);
		glColor3f(0, 1.0, 0);
		DrawCircle(x+r*.15, y+r*.9, r*.4, 10);
		break;
	case 'G':
		glColor3f(1.0, 1.0, 0);
		DrawCircle(x, y, r, 10);
		glColor3f(0, 1.0, 0);
		DrawCircle(x + r * .15, y + r * .9, r*.4, 10);
		break;
	case 'B':
		glColor3f(0,0,0 );
		DrawCircle(x, y, r, 10);
		glColor3f(1, 0, 0);
		DrawRectangle(x - r *.3, y + r, x+ r*.3, y+r*.5);
		break;
	case ' ':
		break;
	default:
		break;
	}
}

void update() {
	bw = !bw;
	bw2 = !bw2;
	if (pauseGame == true)
		return;
	//Generate Fruit...
	if (rand() / (double)RAND_MAX < PGenerateFruit) {
		int x = rand() % gridWidth;
		int y = rand() % gridHeight;

		//Randomly generate golden apples and bombs 10% of the time
		char apple = rand() % 10 == 5 ? (rand() % 2 == 1 ?'G':'B') : 'A';

		if (board[x][y] != 'S' && board[x][y] != 's' && board[x][y] != 'T' && board[x][y] != 't') {
			board[x][y] = apple;
		}
	}

	//Add walls	
	for (int i = 10; i < 40; i++) {
		board[10][i] = 'W';
		board[50][i] = 'W';
	}
	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Find out where the head will be...
	
	switch (direction) {
		case 'N': headY++; break;
		case 'E': headX++; break;
		case 'S': headY--; break;
		case 'W': headX--; break;
	}
	if (headX < 0) headX = gridWidth - 1;
	if (headY < 0) headY = gridHeight - 1;
	if (headX >= gridWidth)  headX = 0;
	if (headY >= gridHeight) headY = 0;

	//Move the first snake's tail..
	if (grow1 == 0) {
		int tailX = snakeX.back();
		int tailY = snakeY.back();
		snakeX.pop_back();
		snakeY.pop_back();
		board[tailX][tailY] = ' ';
	}
	else if (grow1 < 0) {
		int tailX = snakeX.back();
		int tailY = snakeY.back();
		snakeX.pop_back();
		snakeY.pop_back();
		board[tailX][tailY] = ' ';
		if (snakeX.size() > 0 && snakeY.size() > 0) {
			tailX = snakeX.back();
			tailY = snakeY.back();
			snakeX.pop_back();
			snakeY.pop_back();
			board[tailX][tailY] = ' ';
		}
		else
			gameOver = true;
		grow1++;
	}
	else {
		grow1--;
	}

	if (snakeX.size() < 0 && snakeY.size() < 0)
		gameOver = true;

	switch (board[headX][headY])
	{
		case 'A': grow1 += 4; break;
		case 'G': grow1 += 8; break;
		case 'B': grow1 += -6; break;
		case 'S': case 's': case 'W': case 'T': case 't': gameOver = true; break;
	}

	//Move the snake's head.
	board[headX][headY] = bw ? 'S' : 's';
	snakeX.push_front(headX);
	snakeY.push_front(headY);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	switch (direction2) {
		case 'N': head2Y++; break;
		case 'E': head2X++; break;
		case 'S': head2Y--; break;
		case 'W': head2X--; break;
	}
	if (head2X < 0) head2X = gridWidth - 1;
	if (head2Y < 0) head2Y = gridHeight - 1;
	if (head2X >= gridWidth)  head2X = 0;
	if (head2Y >= gridHeight) head2Y = 0;

	//Move the second snake's tail..
	if (grow2 == 0) {
		int tailX = snake2X.back();
		int tailY = snake2Y.back();
		snake2X.pop_back();
		snake2Y.pop_back();
		board[tailX][tailY] = ' ';
	}
	else if (grow2 < 0) {
		int tailX = snake2X.back();
		int tailY = snake2Y.back();
		snake2X.pop_back();
		snake2Y.pop_back();
		board[tailX][tailY] = ' ';
		if (snake2X.size() > 0 && snake2Y.size() > 0) {
			tailX = snake2X.back();
			tailY = snake2Y.back();
			snake2X.pop_back();
			snake2Y.pop_back();
			board[tailX][tailY] = ' ';
		}
		else
			gameOver = true;
		grow2++;
	}
	else {
		grow2--;
	}

	if (snake2X.size() < 0 && snake2Y.size() < 0)
		gameOver = true;

	switch (board[head2X][head2Y])
	{
	case 'A': grow2 += 4; break;
	case 'G': grow2 += 8; break;
	case 'B': grow2 += -6; break;
	case 'S': case 's': case 'W': case 'T': case 't': gameOver = true; break;
	}

	//Move the snake's head.
	board[head2X][head2Y] = bw ? 'T' : 't';
	snake2X.push_front(head2X);
	snake2Y.push_front(head2Y);
}

void draw() {

	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			DrawElement(i, j, board[i][j]);
		}
	}
	Sleep(100);
	
	if (!gameOver)
		update();
}

void WriteGameToFile() {
	string fileLocation;
	cout << "Enter location to save game: ";
	cin >> fileLocation;

	ofstream gameFile;
	gameFile.open(fileLocation);

	//Write board to file
	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			gameFile << board[i][j];
		}
		gameFile << "\n";
	}
	
	//Write game variables to file
	gameFile << gameOver << "\n";
	gameFile << direction << "\n";
	gameFile << direction2 << "\n";
	gameFile << bw << "\n";
	gameFile << bw2 << "\n";
	gameFile << pauseGame << "\n";
	gameFile << headX << "\n";
	gameFile << headY << "\n";
	gameFile << head2X << "\n";
	gameFile << head2Y << "\n";
	gameFile << grow1 << "\n";
	gameFile << grow2 << "\n";
	gameFile << snakeX.size() << "\n";
	gameFile << snake2X.size() << "\n";

	//Write snake 1 to file
	//Write x coordinates
	for (std::list<int>::iterator it = snakeX.begin(); it != snakeX.end(); ++it) {
		gameFile << *it << "\n";
	}
	for (std::list<int>::iterator it = snakeY.begin(); it != snakeY.end(); ++it) {
		gameFile << *it << "\n";
	}
	//Write snake 2 to file
	for (std::list<int>::iterator it = snake2X.begin(); it != snake2X.end(); ++it) {
		gameFile << *it << "\n";
	}
	for (std::list<int>::iterator it = snake2Y.begin(); it != snake2Y.end(); ++it) {
		gameFile << *it << "\n";
	}
	gameFile.close();
	return;
}

void ReadGameFromFile() {
	string fileLocation;
	cout << "Enter location of game to load: ";
	cin >> fileLocation;
	
	ifstream gameFile;
	gameFile.open(fileLocation);

	//Read board from file
	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			gameFile.get(board[i][j]);
		}
		gameFile.get();
	}

	//Read game variables from file
	gameFile >> gameOver;
	gameFile >> direction;
	gameFile >> direction2;
	gameFile >> bw;
	gameFile >> bw2;
	gameFile >> pauseGame;
	gameFile >> headX;
	gameFile >> headY ;
	gameFile >> head2X ;
	gameFile >> head2Y;
	gameFile >> grow1;
	gameFile >> grow2;
	//snake sizes
	int snake1; 
	int snake2;
	gameFile >> snake1;
	gameFile >> snake2;

	std::string line;
	
	//Snake 1 x values
	for (int j = 0; j < snake1; j++) {
		int value;
		gameFile >> value;
		snakeX.push_front(value);
	}
	//Snake 1 y value
	for (int j = 0; j < snake1; j++) {
		int value;
		gameFile >> value;
		snakeY.push_back(value);
	}
	//Snake 2 x values
	for (int j = 0; j < snake2; j++) {
		int value;
		gameFile >> value;
		snake2X.push_back(value);
	}
	//Snake 2 y values
	for (int j = 0; j < snake2; j++) {
		int value;
		gameFile >> value;
		snake2Y.push_back(value);
	}

	gameFile.close();
	update();
}

void keyboard(int key) {
	switch (key) {
		case 'w': if (direction == 'E' || direction == 'W') direction = 'N'; break;
		case 'a': if (direction == 'N' || direction == 'S')	direction = 'W'; break;
		case 's': if (direction == 'E' || direction == 'W')	direction = 'S'; break;
		case 'd': if (direction == 'N' || direction == 'S') direction = 'E'; break;
		case 'i': if (direction2 == 'E' || direction2 == 'W') direction2 = 'N'; break;
		case 'j': if (direction2 == 'N' || direction2 == 'S') direction2 = 'W'; break;
		case 'k': if (direction2 == 'E' || direction2 == 'W') direction2 = 'S'; break;
		case 'l': if (direction2 == 'N' || direction2 == 'S') direction2 = 'E'; break;
		case 'p': pauseGame = !pauseGame; break;
		case 'n': WriteGameToFile(); break;
		case 'm': ReadGameFromFile(); break;
		case ' ':		
			if (gameOver) {
				gameOver = false;
				snakeX.clear();
				snakeY.clear();
				snake2X.clear();
				snake2Y.clear();

				headX = 20;
				headY = 30;
				head2X = 40;
				head2Y = 30;
				grow1 = INITIAL_SIZE;
				grow2 = INITIAL_SIZE;
				direction = 'N';
				direction2 = 'S';

				for (int i = 0; i < gridWidth; i++) {
					for (int j = 0; j < gridHeight; j++) {
						board[i][j] = ' ';
					}
				}			
			}
			break;
	}
}

int main(int argc, char** argv) {
	RunOpenGL(argc, argv, draw, NULL, keyboard);
}