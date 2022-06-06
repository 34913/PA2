#include "Money.h"

Money::Money(uint32_t starting)
	:golds(starting)
{}

void Money::AddMoney(Object& obj)
{
	golds += amount[obj.type.code];
}

uint32_t Money::GetMoney()
{
	return golds;
}

void Money::Print(std::ostream& os)
{
	os << golds << " ";
	MoneyNeeded::Print(os);
}

void Money::Scan(std::istream& is)
{
	if(!(is >> golds) || !is.good())
		throw std::invalid_argument("File corrupted, cant read gold amount");

	try {
		MoneyNeeded::Scan(is);
	}
	catch (const std::invalid_argument& e) {
		throw e;
	}
}

void Money::begin()
{
	amount.clear();
	amount.insert({ Base::type.code, 400 });
	amount.insert({ MeleeAnt::type.code, 20 });
	amount.insert({ RangedAnt::type.code, 25 });
	amount.insert({ TankAnt::type.code, 75 });
}
