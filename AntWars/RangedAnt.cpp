#include "RangedAnt.h"

RangedAnt::RangedAnt(Point start)
	:Ant(start, Stats(2.0, 10.0, 10.0), rangedAntCode)
{}

bool RangedAnt::Attack(Object& obj)
{
	double len = coords.Length(obj.GetCoords());

	double half = values.range / 2;
	if (len > values.range / 2) {

		double percentil = 50 + ((values.range - half) / 50) * (len - half);
		
		srand((unsigned int)time(NULL));
		if (((double)(rand() % 1000) / 10) > percentil)
			return false;
	}

	Object::Attack(obj);

	return true;
}
