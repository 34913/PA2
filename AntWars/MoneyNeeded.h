#pragma once

#include "Base.h"
#include "RangedAnt.h"
#include "MeleeAnt.h"
#include "TankAnt.h"

#include "Object.h"

#include <map>

class MoneyNeeded
{
protected:
	
	std::map<int, int> amount;

	/**
	 * Adds on start basic objects and their price/cost (depends on class using)
	 *
	 */
	virtual void Add();

public:

	MoneyNeeded();

	virtual void Up();

	int GetAmount(Object& obj);

	int GetAmount(TypeCode& type);

};

