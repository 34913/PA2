#include "MoneyNeeded.h"

void MoneyNeeded::Add()
{
	//amounts.insert({ Base::baseType.code, 800 });
	amount.insert({ MeleeAnt::meleeAntCode.code, 40 });
	amount.insert({ RangedAnt::rangedAntCode.code, 50 });
	amount.insert({ TankAnt::tankAntCode.code, 150 });
}

MoneyNeeded::MoneyNeeded()
{
	Add();
}

void MoneyNeeded::Up()
{
	for (auto& x : amount) {
		amount[x.first] = x.second * 2;
	}
}

uint16_t MoneyNeeded::operator[](Object& obj) 
{
	return amount[obj.type.code];
}

uint16_t MoneyNeeded::operator[](TypeCode& type)
{
	return amount[type.code];
}
