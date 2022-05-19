#include "Ant.h"

const Point& Ant::Move(const Point& dir)
{
	// maybe doing something to walk on diagonal and straight way at the same time
	// over the moves

	// -> now just first diagonaly and after that straight

	if (dir.x > coords.x)
		coords.Right();
	else if (dir.x != coords.x)
		coords.Left();

	if (dir.y > coords.y)
		coords.Up();
	else if (dir.y != coords.y)
		coords.Down();

	return coords;
}
