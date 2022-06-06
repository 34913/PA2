#pragma once

#include <fstream>

class Stats
{
public:

	double damage;
	double range;
	double health;

	Stats(double d, double r, double h);

	Stats(const Stats& s);

	/**
	 * adds certain amount of stats to this.
	 * 
	 * \param add to be added
	 */
	void Add(const Stats& add);
	
	friend std::ostream& operator << (std::ostream& os, const Stats& s);

};
