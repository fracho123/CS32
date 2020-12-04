#include <iostream>
#include <stack>
#include <string>
using namespace std;


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	maze[sr][sc] = 'o';
	if (sr <= 0 || sr + 1 >= nRows || sc <= 0 || sc + 1 >= nCols ||
		er <= 0 || er + 1 >= nRows || ec <= 0 || ec + 1 >= nCols)
		return false;
	if (sr == er && sc == ec)
		return true;
	if (maze[sr][sc + 1] == '.') { //right
		maze[sr][sc + 1] = 'o';
		if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
			return true;
	}
	if (maze[sr + 1][sc] == '.') { //down
		maze[sr + 1][sc] = 'o';
		if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
			return true;
	}
	if (maze[sr][sc - 1] == '.') { //left
		maze[sr][sc - 1] = 'o';
		if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
			return true;
	}
	if (maze[sr - 1][sc] == '.') {//up
		maze[sr - 1][sc] = 'o';
		if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
			return true;
	}
	return false;
}


int main()
{
	string maze[10] = {
	  "XXXXXXXXXX",
	  "XXXXXXX..X",
	  "XX.....X.X",
	  "X..X....XX",
	  "X..X..XXXX",
	  "X..XXX.X.X",
	  "XX...X...X",
	  "X....XX..X",
	  "XXX.....XX",
	  "XXXXXXXXXX",
	};

	if (pathExists(maze, 10, 10, 5, 6, 2, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}