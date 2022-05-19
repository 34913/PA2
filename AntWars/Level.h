#pragma once

#include "Ant.h"
#include "TankAnt.h"
#include "RangedAnt.h"
#include "MeleeAnt.h"
#include "Base.h"

#include <map>

class Level
{
private:

	std::map<int, int> amount;

	int exp;
	uint16_t level;

	const int limit;

public:

	/**
	 * constructor.
	 * 
	 * \param startingExp amount of experience on the beggining, default = 0
	 * \param level to start with, default = 1
	 */
	Level(int startingExp = 0, int level = 1);


	/**
	 * add expierence.
	 * 
	 * \param type of killed object, determines the amount of points
	 * \return true if level up is present
	 */
	bool AddExp(const Object* type);

	/**
	 * returns level.
	 * 
	 * \return int value of level
	 */
	int GetLevel();

};

