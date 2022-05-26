
#include "Point.h"

bool Point::operator==(const Point& p) const
{
	return p.x == x && p.y == y;
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
	os << p.x << " " << p.y;
	return os;
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

double Point::Length(Point& diff)
{
	return Length(*this, diff);
}

double Point::Length(Point& a, Point& b)
{
	double temp[2] = { 0,0 };
	temp[0] = a.x - b.x;
	temp[1] = a.y - b.y;

	double len = pow(temp[0], 2) + pow(temp[1], 2);

	// count the length from one to another
	// classic triangle
	return sqrt(len);
}
