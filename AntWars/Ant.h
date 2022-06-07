#ifndef Ant_H
#define Ant_H

#include "Point.h"
#include "Stats.h"
#include "Object.h"

class Ant : public Object
{
public:

	Ant(const Point& coords, const Stats& values, const TypeCode& type);
	
	/**
	 * Moving the ant.
	 * 
	 * \param dir direction to which point move
	 * \return point where it is now
	 */
	virtual Point& Move(const Point& dir);

};

#endif
