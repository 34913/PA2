#ifndef RangedAnt_H
#define RangedAnt_H

#include "Ant.h"
#include "Point.h"
#include "Stats.h"
#include "TypeCode.h"

class RangedAnt : public Ant
{
	// worst accuracy (in %) you can get
	// can be set up in constructor
	const double worst;

public:

	static TypeCode type;

	RangedAnt(Point start = Point(0, 0));

	bool Attack(Object& obj) override;

};

#endif
