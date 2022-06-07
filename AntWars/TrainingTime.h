#ifndef TrainingTime_H
#define TrainingTime_H

#include "MoneyNeeded.h"
#include "MeleeAnt.h"
#include "RangedAnt.h"
#include "TankAnt.h"
#include "Base.h"

class TrainingTime : public MoneyNeeded
{
public:

	TrainingTime();

	void begin() override;

};

#endif
