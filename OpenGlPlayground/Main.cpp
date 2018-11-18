#include "OpenGL.h"
#include "GL/glut.h"
#include "GL/GL.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <ctime>
#include <Windows.h>

using namespace std;
const int gridHeight = 50;
const int gridWidth = 60;
const int INITIAL_SIZE = 5;
const double PGenerateFruit = .5;

bool bw = false;
bool pauseGame = false;
int headX = 15;
int headY = 25;
int head2X = 30;
int head2Y = 25;
int grow1 = INITIAL_SIZE;
int grow2 = INITIAL_SIZE;

list<int> snakeX;
list<int> snakeY;

list<int> snake2X;
list<int> snake2Y;

char board[gridWidth][gridHeight];
char direction = 'E';
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
	if (pauseGame == true)
		return;
	//Generate Fruit...
	if (rand() / (double)RAND_MAX < PGenerateFruit) {
		int x = rand() % gridWidth;
		int y = rand() % gridHeight;

		//Randomly generate golden apples and bombs 10% of the time
		char apple = rand() % 10 == 5 ? (rand() % 2 == 1 ?'G':'B') : 'A';

		if (board[x][y] != 'S' && board[x][y] != 's') {
			board[x][y] = apple;
		}
	}

	//Add walls
	for (int i = 10; i < 40; i++) {
		board[10][i] = 'W';
		board[50][i] = 'W';
	}

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
	////Move the second snake's tail..
	//if (grow2 == 0) {
	//	int tailX = snake2X.back();
	//	int tailY = snake2Y.back();
	//	snake2X.pop_back();
	//	snake2Y.pop_back();
	//	board[tailX][tailY] = ' ';
	//}
	//else if (grow2 < 0) {
	//	int tailX = snake2X.back();
	//	int tailY = snake2Y.back();
	//	snake2X.pop_back();
	//	snake2Y.pop_back();
	//	board[tailX][tailY] = ' ';
	//	if (snake2X.size() > 0 && snake2Y.size() > 0) {
	//		tailX = snake2X.back();
	//		tailY = snake2Y.back();
	//		snake2X.pop_back();
	//		snake2Y.pop_back();
	//		board[tailX][tailY] = ' ';
	//	}
	//	else
	//		gameOver = true;
	//	grow2++;
	//}
	//else {
	//	grow2--;
	//}

 	if (snakeX.size() < 0 && snakeY.size() < 0)
		gameOver = true;
	if (snake2X.size() < 0 && snake2Y.size() < 0)
		gameOver = true;

	switch (board[headX][headY])
	{
		case 'A': grow1 += 4; break;
		case 'G': grow1 += 8; break;
		case 'B': grow1 += -6; break;
		case 'S': case 's': case 'W': gameOver = true; break;
	}

	//Move the snake's head.
	board[headX][headY] = bw ? 'S' : 's';
	snakeX.push_front(headX);
	snakeY.push_front(headY);

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

void keyboard(int key) {
	switch (key) {
		case 'w': case 'i': if (direction == 'E' || direction == 'W') direction = 'N'; break;
		case 'a': case 'j': if (direction == 'N' || direction == 'S')	direction = 'W'; break;
		case 's': case 'k': if (direction == 'E' || direction == 'W')	direction = 'S'; break;
		case 'd': case 'l': if (direction == 'N' || direction == 'S') direction = 'E'; break;
		case 'p': pauseGame = !pauseGame; break;
		case ' ':		
			if (gameOver) {
				gameOver = false;
				snakeX.clear();
				snakeY.clear();
				snake2X.clear();
				snake2X.clear();

				headX = gridWidth / 2.0;
				headY = gridHeight / 2.0;
				for (int i = 0; i < gridWidth; i++) {
					for (int j = 0; j < gridHeight; j++) {
						board[i][j] = ' ';
					}
				}
				grow1 = INITIAL_SIZE;
				grow2 = INITIAL_SIZE;
			}
			break;
	}
}

int main(int argc, char** argv) {
	RunOpenGL(argc, argv, draw, NULL, keyboard);
}