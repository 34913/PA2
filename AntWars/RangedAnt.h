#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"

const TypeCode rangedAntCode = TypeCode(2);

class RangedAnt : public Ant
{
protected:

public:

	RangedAnt()
		:Ant(Point(0, 0), Stats(2, 5, 10), rangedAntCode)
	{}

	RangedAnt(Point start)
		:Ant(start, Stats(2, 5, 10), rangedAntCode)
	{}
};
