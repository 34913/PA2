#ifndef Point_H
#define Point_H

#include <fstream>
#include <math.h>

class Point
{
public:
	int x, y;
	
	static Point dirs[];

	/**
	 * Returns Dir from dirs based on index, which can be out of bounds.
	 *	-> it is recalculated
	 *
	 * \return dir ref based on index
	 */
	static Point& GetDir(int index);

	// constructors
	
	Point(int x, int y);

	Point(const Point& a);

	// operators

	bool operator == (const Point& p) const;

	bool operator != (const Point& p) const;

	friend std::ostream& operator << (std::ostream& os, const Point& x);

	Point operator + (const Point& obj);

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
	 * Finds the direction and returns index from dirs.
	 * 
	 * \param absDir point which you are headed
	 * \return index of dirs
	 */
	int FindDir(const Point& absDir);

	/**
	 * Turns relatively right on where you are headed
	 * so if you are [0,0] and dir is [0,1] -> this gets result [1,1]
	 *	-> once more and you are [1,0]
	 * 
	 * \param dir where you are headed
	 */
	void TurnRight(int& dir);

	/**
	 * Turns relatively right on where you are headed
	 * so if you are [0,0] and dir is [0,1] -> this gets result [-1,1]
	 *	-> once more and you are [-1,0]
	 * 
	 * \param dir where you are headed
	 */
	void TurnLeft(int& dir);

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

#endif
