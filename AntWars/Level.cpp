#include "Level.h"

Level::Level(int startingExp, int Startinglevel)
	:exp(startingExp),
	level(Startinglevel),
	lastLevel(1),
	limit(1000)
{}

bool Level::AddExp(Object& obj)
{
	if(amount.find(obj.type.code) == amount.end())
		throw std::invalid_argument("not known type of object");
	exp += amount[obj.type.code];

	if (exp > limit) {
		level++;
		exp -= limit;

		return true;
	}
	return false;
}

int Level::GetLevel()
{
	return level;
}

int Level::CheckLevel()
{
	int changes = (level - lastLevel);
	lastLevel = level;
	return changes;
}

void Level::Print(std::ostream& os)
{
	os << exp << " " << level << " ";
	MoneyNeeded::Print(os);
}

void Level::Scan(std::istream& is)
{
	if (!(is >> exp >> level) || !is.good())
		throw std::invalid_argument("File corrupted, cant read exp and level");

	try {
		MoneyNeeded::Scan(is);
	}
	catch (const std::invalid_argument& e) {
		throw e;
	}
}

void Level::begin()
{
	amount.clear();
	amount.insert({ MeleeAnt::type.code, 10 });
	amount.insert({ RangedAnt::type.code, 15 });
	amount.insert({ TankAnt::type.code, 30 });
	amount.insert({ Base::type.code, 500 });
}
