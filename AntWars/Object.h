#pragma once

#include "Point.h"
#include "Stats.h"
#include "TypeCode.h"
#include "UniqID.h"

#include <math.h>

class Object
{
protected:

	Point coords;

	Stats values;

public:

	const long id;

	const TypeCode& type;

	Object(const Point& coords, const Stats& values, const TypeCode& type);
	
	~Object();

	bool InRange(const Object& obj, double& len) const;

	virtual void Attack(Object& obj);

	virtual void Defend(const int dmg);

	virtual bool IsAlive() const;

	Point GetCoords() const;

	void AddStats(const Stats& add);

};
