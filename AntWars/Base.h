#pragma once

#include "Object.h"
#include "Point.h"
#include "Stats.h"

#include "TypeCode.h"


class Base : public Object
{
public:

	static TypeCode baseType;

	Base(const Point& coords = Point(0,0));

};
