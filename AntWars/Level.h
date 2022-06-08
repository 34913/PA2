#ifndef Level_H
#define Level_H

#include "Object.h"

#include "MeleeAnt.h"
#include "RangedAnt.h"
#include "TankAnt.h"
#include "Base.h"

#include "MoneyNeeded.h"

#include <map>

class Level : public MoneyNeeded
{
	int exp;
	int level;
	int lastLevel = 1;

	// max exp on this level
	int limit = 1000;

	void Print(std::ostream& os) override;

	void Scan(std::istream& is) override;

public:

	/**
	 * constructor.
	 *
	 * \param startingExp amount of experience on the beggining, default = 0
	 * \param startingLevel to start with, default = 1
	 */
	Level(int startingExp = 0, int startingLevel = 1);

	/**
	 * add expierence.
	 * 
	 * \param obj of killed object, determines the amount of points
	 * \return true if level up is present
	 */
	bool AddExp(const Object& obj);

	/**
	 * returns level.
	 * 
	 * \return int value of level
	 */
	int GetLevel();

	/**
	 * Checks the change of level.
	 * 
	 * \return the amount of changes of level
	 */
	int CheckLevel();

	/**
	 * Adds all the assets in set, need to be called first after init.
	 * 
	 */
	void begin() override;

	void Up() override;

};

#endif
