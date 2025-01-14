#include "TankAnt.h"

TypeCode TankAnt::type(4);

TankAnt::TankAnt(Point start)
	:Ant(start, Stats(1.0, 1.5, 50.0), type)
{}

bool TankAnt::Defend(Object& obj)
{
	int chance = rand() % 100;
	if (chance > 95)
		return false;

	Object::Defend(obj);
	return true;
}
