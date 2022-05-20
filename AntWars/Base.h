#pragma once

#include "Object.h"
#include "Point.h"
#include "Stats.h"

#include "TypeCode.h"

const TypeCode baseType = TypeCode(1);

class Base : public Object
{
public:

	Base(const Point& coords);

};
