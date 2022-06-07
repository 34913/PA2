#ifndef Base_H
#define Base_H

#include "Object.h"
#include "Point.h"
#include "Stats.h"

#include "TypeCode.h"

class Base : public Object
{
public:

	static TypeCode type;

	Base(const Point& coords = Point(0, 0));

};

#endif
