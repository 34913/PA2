#include "TankAnt.h"

TypeCode TankAnt::tankAntCode(4);

TankAnt::TankAnt(Point start)
	:Ant(start, Stats(1.0, 1.5, 25.0), tankAntCode)
{}

bool TankAnt::Defend(Object& obj)
{
	srand((unsigned int)time(NULL));
	int chance = rand() % 100;
	if (chance > 95)
		return false;

	Object::Defend(obj);
	return true;
}
