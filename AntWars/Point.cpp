
#include "Point.h"

bool Point::operator==(const Point& p) const
{
	return p.x == x && p.y == y;
}

void Point::operator = (const Point& p)
{
	this->x = p.x;
	this->y = p.y;
}

void Point::Right()
{
	x++;
}

void Point::Left()
{
	x--;
}

void Point::Down()
{
	y++;
}

void Point::Up()
{
	y--;
}
