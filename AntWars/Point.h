#pragma once

#include <fstream>

class Point
{
public:
	int x, y;

	// constructors
	
	Point(int x, int y);

	Point(const Point& a);

	// operators

	bool operator == (const Point& p) const;

	bool operator != (const Point& p) const;

	friend std::ostream& operator << (std::ostream& os, const Point& x);

	// methods

	/**
	 * Move the coordinations to right.
	 * 
	 */
	void Right();

	/**
	 * Move the coordinations to left.
	 * 
	 */
	void Left();

	/**
	 * Move the coordinations down.
	 * 
	 */
	void Down();

	/**
	 * Move the coordinations up.
	 * 
	 */
	void Up();

	/**
	 * Count the length of this and given point.
	 * 
	 * \param diff the given point
	 * \return length betweeen these two in double
	 */
	double Length(Point& diff);

	/**
	 * Counts the length between two given points.
	 * 
	 * \param a first given point
	 * \param b second given point
	 * \return the distance between these two
	 */
	static double Length(Point& a, Point& b);

};
