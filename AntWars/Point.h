#pragma once

#include <ostream>

class Point
{
public:
	int x, y;

	Point(int x, int y)
		:x(x), y(y)
	{}

	Point(const Point& a)
		:x(a.x), y(a.y)
	{}

	bool operator == (const Point& p) const;

	void operator = (const Point& p);

	friend std::ostream& operator << (std::ostream& os, const Point& x);

	void Right();

	void Left();

	void Down();

	void Up();

};
