#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"

#include "TypeCode.h"

class MeleeAnt: public Ant
{
	// counters and limit for attack
	// -> charged up attack, deals more damage
	int counter;
	const int limit;

public:
	
	static TypeCode type;

	MeleeAnt(Point start = Point(0, 0));

	bool Attack(Object& obj) override;

};
