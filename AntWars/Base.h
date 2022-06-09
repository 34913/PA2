#ifndef Base_H
#define Base_H

#include "Object.h"
#include "Point.h"
#include "Stats.h"

#include "TypeCode.h"

class Base : public Object
{

	int counter = 0;
	const int add = 2;

public:

	static TypeCode type;

	Base(const Point& coords = Point(0, 0));

	bool Attack(Object& obj) override;

};

#endif
