#pragma once

#include "Point.h"
#include "Stats.h"
#include "Object.h"

class Ant : Object
{
public:

	Ant(const Point& coords, const Stats& values, const TypeCode& type)
		:Object(coords, values, type)
	{}

	virtual const Point& Move(const Point& dir);

};

