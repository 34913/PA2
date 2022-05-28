#include "Money.h"

void Money::Add()
{
	amount.insert({ Base::baseType.code, 400 });
	amount.insert({ MeleeAnt::meleeAntCode.code, 20 });
	amount.insert({ RangedAnt::rangedAntCode.code, 25 });
	amount.insert({ TankAnt::tankAntCode.code, 75 });
}

Money::Money(uint32_t starting)
	:golds(starting),
	MoneyNeeded()
{}

void Money::AddMoney(Object& obj)
{
	golds += amount[obj.type.code];
}

uint32_t Money::GetMoney()
{
	return golds;
}
