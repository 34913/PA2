#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"

#include "TypeCode.h"

const TypeCode meleeAntCode = TypeCode(3);

class MeleeAnt: public Ant
{
public:

	MeleeAnt(Point start = Point(0,0))
		:Ant(start, Stats(3, 1, 15), meleeAntCode)
	{}

};
