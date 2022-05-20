#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"
#include "TypeCode.h"

const TypeCode tankAntCode = TypeCode(4);

class TankAnt : public Ant
{
protected:

public:

	TankAnt()
		:Ant(Point(0, 0), Stats(1, 1, 25), tankAntCode)
	{}

	TankAnt(Point start)
		:Ant(start, Stats(1, 1, 25), tankAntCode)
	{}

};
