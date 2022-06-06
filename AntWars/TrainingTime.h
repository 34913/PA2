#pragma once

#include "MoneyNeeded.h"

class TrainingTime : public MoneyNeeded
{
public:

	TrainingTime();

	void begin() override;

};
