#include "support.h"
#include <string>
using namespace std;

//returns the string name for an analog direction
string compassDir(const double& direction) {
	if (direction >= 0 && direction < 22.5)
		return "east";
	else if (direction >= 22.5 && direction < 67.5)
		return "northeast";
	else if (direction >= 67.5 && direction < 112.5)
		return "north";
	else if (direction >= 112.5 && direction < 157.5)
		return "northwest";
	else if (direction >= 157.5 && direction < 202.5)
		return "west";
	else if (direction >= 202.5 && direction < 247.5)
		return "southwest";
	else if (direction >= 247.5 && direction < 292.5)
		return "south";
	else if (direction >= 292.5 && direction < 337.5)
		return "southeast";
	return "east";
}