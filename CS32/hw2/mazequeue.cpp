#include <iostream>
#include <queue>
#include <string>
using namespace std;

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	queue<Coord> coordQueue;
	Coord a(sr, sc);
	coordQueue.push(a);
	maze[sr][sc] = 'o';
	int r = sr, c = sc;
	if (sr <= 0 || sr + 1 >= nRows || sc <= 0 || sc + 1 >= nCols ||
		er <= 0 || er + 1 >= nRows || ec <= 0 || ec + 1 >= nCols)
		return false;
	while (!coordQueue.empty()) {
		Coord b = coordQueue.front();
		coordQueue.pop();
		r = b.r();
		c = b.c();
		if (r == er && c == ec)
			return true;
		if (maze[r][c + 1] == '.') {
			Coord d(r, c + 1);
			coordQueue.push(d);
			maze[r][c + 1] = 'o';
		}
		if (maze[r + 1][c] == '.') {
			Coord e(r + 1, c);
			coordQueue.push(e);
			maze[r + 1][c] = 'o';
		}
		if (maze[r][c - 1] == '.') {
			Coord f(r, c - 1);
			coordQueue.push(f);
			maze[r][c - 1] = 'o';
		}
		if (maze[r - 1][c] == '.') {
			Coord g(r - 1, c);
			coordQueue.push(g);
			maze[r - 1][c] = 'o';
		}
	}
	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X...X..X.X",
		"X.XXX....X",
		"X.X.XXXX.X",
		"XXX......X",
		"X...X.XX.X",
		"X.X.X..X.X",
		"X.XXXX.X.X",
		"X..X...X.X",
		"XXXXXXXXXX"
	};

	if (pathExists(maze, 10, 10, 4, 3, 1, 8))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;
}