#include "MeleeAnt.h"

TypeCode MeleeAnt::type(3);

MeleeAnt::MeleeAnt(Point start)
	:Ant(start, Stats(3.0, 1.5, 30.0), MeleeAnt::type)
{}

bool MeleeAnt::Attack(Object& obj)
{
	if (!Object::Attack(obj))
		return false;

	if (!obj.IsAlive() && counter != limit) {
		if (++counter == limit)
			values.Add(Stats(2, 0, 0));
	}

	return true;
}
