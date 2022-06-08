#include "Point.h"

Point Point::dirs[] = {
	Point( 0,-1),
	Point( 1,-1),
	Point( 1, 0),
	Point( 1, 1),
	Point( 0, 1),
	Point(-1, 1),
	Point(-1, 0),
	Point(-1,-1)
};

Point& Point::GetDir(int index)
{
	while (index < 0 || index > 7) {
		if (index < 0)
			index += 8;
		else if (index > 7)
			index -= 7;
	}

	return Point::dirs[index];
}

Point::Point(int x, int y)
	:x(x), y(y)
{}

Point::Point(const Point& a)
	:x(a.x), y(a.y)
{}

bool Point::operator==(const Point& p) const
{
	return p.x == x && p.y == y;
}

bool Point::operator!=(const Point& p) const
{
	return !(*this == p);
}

Point Point::operator+(const Point& obj)
{
	return Point(x + obj.x, y + obj.y);
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

int Point::FindDir(const Point& absDir)
{
	Point find(absDir.x - x, absDir.y - y);

	for (int i = 0; i < 8; i++) {
		if (find == dirs[i])
			return i;
	}
	throw std::invalid_argument("Not in dirs, you didnt entered one of direction");
}

void Point::TurnRight(int& dir)
{
	if (++dir == 8)
		dir = 0;

	x += dirs[dir].x;
	y += dirs[dir].y;
}

void Point::TurnLeft(int& dir)
{
	if (--dir == 0)
		dir = 7;

	x += dirs[dir].x;
	y += dirs[dir].y;
}

double Point::Length(Point& diff)
{
	return Length(*this, diff);
}

double Point::Length(Point& a, Point& b)
{
	// count the length from one to another
	// classic triangle
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}
