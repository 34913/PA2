#pragma once

class Stats
{
public:

	int damage;
	int range;
	int health;

	Stats(int d, int r, int h)
		:damage(d),
		range(r),
		health(h)
	{}

	Stats(const Stats& s)
		:damage(s.damage),
		range(s.range),
		health(s.health)
	{}

};
