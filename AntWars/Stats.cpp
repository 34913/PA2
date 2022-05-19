#include "Stats.h"

void Stats::Add(const Stats& add)
{
	damage += add.damage;
	range += add.damage;
	health += add.health;
}
