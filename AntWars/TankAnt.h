#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"
#include "TypeCode.h"

class TankAnt : public Ant
{
public:

	static TypeCode tankAntCode;

	TankAnt(Point start = Point(0, 0));

	bool Defend(Object& obj) override;

};
