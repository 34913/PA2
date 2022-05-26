#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"
#include "TypeCode.h"

const TypeCode tankAntCode = TypeCode(4);

class TankAnt : public Ant
{
public:

	TankAnt(Point start = Point(0, 0));

	bool Defend(Object& obj) override;

};
