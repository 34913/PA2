#pragma once

#include "Point.h"
#include "Stats.h"
#include "Object.h"

class Ant : public Object
{
public:

	Ant(const Point& coords, const Stats& values, const TypeCode& type);

	virtual Point& Move(const Point& dir);

};
