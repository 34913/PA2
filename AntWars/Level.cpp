#include "Level.h"

void Level::Add()
{
	amount.insert({ MeleeAnt::meleeAntCode.code, 10 });
	amount.insert({ RangedAnt::rangedAntCode.code, 15 });
	amount.insert({ TankAnt::tankAntCode.code, 30 });
	amount.insert({ Base::baseType.code, 500 });
}

Level::Level(int startingExp, int Startinglevel)
	:exp(startingExp),
	level(1),
	lastLevel(1),
	limit(1000),
	MoneyNeeded()
{
	while (exp > limit) {
		level++;
		exp -= limit;
	}
}

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
