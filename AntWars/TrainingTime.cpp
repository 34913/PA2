#include "TrainingTime.h"

void TrainingTime::Add()
{
	amount.insert({ MeleeAnt::meleeAntCode.code, 1000 });
	amount.insert({ RangedAnt::rangedAntCode.code, 1000 });
	amount.insert({ TankAnt::tankAntCode.code, 1000 });
}

TrainingTime::TrainingTime()
	:MoneyNeeded()
{}
