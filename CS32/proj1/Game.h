#ifndef GAME
#define GAME

#include <string>
using namespace std;

class Arena;

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nVampires);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
};

#endif