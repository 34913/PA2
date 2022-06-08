#include "Ant.h"

Ant::Ant(const Point& coords, const Stats& values, const TypeCode& type)
	:Object(coords, values, type)
{}

Point& Ant::Move(const Point& dir)
{
	if (abs(dir.x - coords.x) > 1 || abs(dir.y - coords.y) > 1)
		throw std::invalid_argument("Point not in range");

	coords = Point(dir);

	return coords;
}

void Ant::TryMove(const Point& dir, Point& possible)
{
	possible.x = coords.x;
	possible.y = coords.y;

	if (dir.x > coords.x)
		possible.Right();
	else if (dir.x != coords.x)
		possible.Left();

	if (dir.y > coords.y)
		possible.Down();
	else if (dir.y != coords.y)
		possible.Up();

}
