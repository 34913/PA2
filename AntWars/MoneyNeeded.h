#pragma once

#include "Base.h"
#include "RangedAnt.h"
#include "MeleeAnt.h"
#include "TankAnt.h"

#include "Object.h"

#include <map>

#include <cstdint>

class MoneyNeeded
{
protected:
	
	std::map<int, uint16_t> amount;

	/**
	 * Adds on start basic objects and their price/cost (depends on class using)
	 *
	 */
	virtual void Add();

public:

	MoneyNeeded();

	virtual void Up();

	uint16_t operator [](Object& obj);
	
	uint16_t operator[](TypeCode& type);

};

