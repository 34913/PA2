#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"

#include "TypeCode.h"

const TypeCode meleeAntCode = TypeCode(3);

class MeleeAnt: public Ant
{
protected:

public:

	MeleeAnt()
		:Ant(Point(0, 0), Stats(3, 1, 15), meleeAntCode)
	{}

	MeleeAnt(Point start)
		:Ant(start, Stats(3, 1, 15), meleeAntCode)
	{}

};
