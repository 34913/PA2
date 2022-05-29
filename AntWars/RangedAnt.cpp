#include "RangedAnt.h"

TypeCode RangedAnt::rangedAntCode(2);

RangedAnt::RangedAnt(Point start)
	:Ant(start, Stats(2.0, 10.0, 10.0), rangedAntCode),
	worst(31.24)
{}

bool RangedAnt::Attack(Object& obj)
{
	double len = coords.Length(obj.GetCoords());
	double half = values.range / 2;

	if (len > half) {
		double percentil = 100 + (worst / (values.range - half)) * (half - len);

		srand((unsigned int)time(NULL) + rand());
		double chance = (rand() % 100) + ((double)(rand() % 101) / 100);

		if (percentil < chance)
			return false;
	}

	Object::Attack(obj);

	return true;
}
