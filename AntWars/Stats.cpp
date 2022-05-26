#include "Stats.h"

Stats::Stats(double d, double r, double h)
	:damage(d),
	range(r),
	health(h)
{}

Stats::Stats(const Stats& s)
	:damage(s.damage),
	range(s.range),
	health(s.health)
{}

void Stats::Add(const Stats& add)
{
	damage += add.damage;
	range += add.damage;
	health += add.health;
}

std::ostream& operator<<(std::ostream& os, const Stats& s)
{
	os << ".d" << s.damage << " h" << s.health << " r" << s.range;
	return os;
}
