#pragma once

#include "Point.h"
#include "Stats.h"
#include <math.h>
#include "TypeCode.h"

class Object
{
protected:

	Point coords;

	Stats values;

public:

	const TypeCode& type;

	Object(const Point& coords, const Stats& values, const TypeCode& type)
		:coords(coords),
		values(values),
		type(type)
	{}
	
	~Object()
	{}

	bool InRange(const Object& obj) const;

	virtual void Attack(Object& obj);

	virtual void Defend(const int dmg);

	virtual bool IsAlive() const;

	const Point& GetCoords() const;

	void AddStats(const Stats& add);

};

