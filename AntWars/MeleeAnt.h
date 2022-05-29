#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"

#include "TypeCode.h"


class MeleeAnt: public Ant
{
	int counter;
	const int limit;

public:
	
	static TypeCode meleeAntCode;

	MeleeAnt(Point start = Point(0, 0));

	bool Attack(Object& obj) override;

};
