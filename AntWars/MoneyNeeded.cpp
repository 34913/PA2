#include "MoneyNeeded.h"

uint16_t MoneyNeeded::operator[](Object& obj) 
{
	return amount[obj.type.code];
}

uint16_t MoneyNeeded::operator[](TypeCode& type)
{
	return amount[type.code];
}

uint16_t MoneyNeeded::operator[](int code)
{
	return amount[code];
}

std::ostream& operator<<(std::ostream& os, MoneyNeeded& obj)
{
	obj.Print(os);
	return os;
}

std::istream& operator>>(std::istream& is, MoneyNeeded& obj)
{
	obj.Scan(is);
	return is;
}

void MoneyNeeded::Up()
{
	for (auto& x : amount) {
		amount[x.first] = x.second * 2;
	}
}

void MoneyNeeded::Print(std::ostream& os)
{
	os << amount.size() << std::endl;
	for (auto& x : amount)
		os << x.first << " " << x.second << std::endl;
}

void MoneyNeeded::Scan(std::istream& is)
{
	int count;
	is >> count;
	
	int tempI;
	uint16_t tempU;
	for(int i = 0; i < count; i++) {
		if (!is.good() || !(is >> tempI >> tempU))
			throw std::invalid_argument("Corrupted file");

		amount.insert(std::make_pair(tempI, tempU));
	}
}

void MoneyNeeded::begin()
{
	amount.clear();
	amount.insert({ MeleeAnt::type.code, 40 });
	amount.insert({ RangedAnt::type.code, 50 });
	amount.insert({ TankAnt::type.code, 150 });
}
