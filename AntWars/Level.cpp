#include "Level.h"

Level::Level(int startingExp, int Startinglevel)
	:exp(startingExp),
	level(level),
	limit(1000)
{
	while (exp > limit) {
		level++;
		exp -= limit;
	}

	amount.insert({ meleeAntCode.code, 10 });
	amount.insert({ rangedAntCode.code, 15 });
	amount.insert({ tankAntCode.code, 30 });
	amount.insert({ baseType.code, 500 });
}

bool Level::AddExp(const Object* type)
{
	if(amount.find(type->type.code) == amount.end())
		throw std::exception("not known type of object");
	exp += amount[type->type.code];

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
