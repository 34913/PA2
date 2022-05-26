#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"

#include "TypeCode.h"

const TypeCode meleeAntCode = TypeCode(3);

class MeleeAnt: public Ant
{
	int counter;
	const int limit;

public:

	MeleeAnt(Point start = Point(0, 0));

	bool Attack(Object& obj) override;

};
