#pragma once

#include "Ant.h"
#include "Point.h"
#include "Stats.h"

const TypeCode rangedAntCode = TypeCode(2);

class RangedAnt : public Ant
{
	// worst accuracy (in %) you can get
	// can be set up in constructor
	const double worst;
public:

	RangedAnt(Point start = Point(0, 0));

	bool Attack(Object& obj) override;

};
