#pragma once

#include "MoneyNeeded.h"

#include <map>

class Level : public MoneyNeeded
{

	int exp;
	int level;
	int lastLevel;

	const int limit;

	void Add() override;

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
	 * \param obj of killed object, determines the amount of points
	 * \return true if level up is present
	 */
	bool AddExp(Object& obj);

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

};
