#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols) {
	m_rows = nRows;
	m_cols = nCols;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			m_grid[r][c] = '.';
}

bool History::record(int r, int c) {
	if (r <= 0 || c <= 0 || r > MAXROWS || c > MAXCOLS)
		return false;
	switch (m_grid[r][c]) {
	case '.':	m_grid[r][c] = 'A'; break;
	case 'Z':	m_grid[r][c] = 'Z'; break;
	default:	m_grid[r][c]++;		break;
	}
	return true;
}

void History::display() const {
	clearScreen();
	for (int r = 1; r <= m_rows; r++)
	{
		for (int c = 1; c <= m_cols; c++)
			cout << m_grid[r][c];
		cout << endl;
	}
	cout << endl;
}