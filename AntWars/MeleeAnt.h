#ifndef MeleeAnt_H
#define MeleeAnt_H

#include "Ant.h"
#include "Point.h"
#include "Stats.h"

#include "TypeCode.h"

class MeleeAnt: public Ant
{
	// counters and limit for attack
	// -> charged up attack, deals more damage
	int counter = 0;
	const int limit = 2;

public:
	
	static TypeCode type;

	MeleeAnt(Point start = Point(0, 0));

	bool Attack(Object& obj) override;

};

#endif
