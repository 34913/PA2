#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"

const TypeCode rangedAntCode = TypeCode(2);

class RangedAnt : public Ant
{
protected:

public:

	RangedAnt(Point start = Point(0, 0))
		:Ant(start, Stats(2, 5, 10), rangedAntCode)
	{}
};
