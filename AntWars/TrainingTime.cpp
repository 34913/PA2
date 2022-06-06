#include "TrainingTime.h"

TrainingTime::TrainingTime()
{}

void TrainingTime::begin()
{
	amount.clear();
	amount.insert({ MeleeAnt::type.code, 1000 });
	amount.insert({ RangedAnt::type.code, 1000 });
	amount.insert({ TankAnt::type.code, 1000 });
}
