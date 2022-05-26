#pragma once

class Stats
{
public:

	double damage;
	double range;
	double health;

	Stats(double d, double r, double h)
		:damage(d),
		range(r),
		health(h)
	{}

	Stats(const Stats& s)
		:damage(s.damage),
		range(s.range),
		health(s.health)
	{}

	void Add(const Stats& add);

};
