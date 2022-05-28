#pragma once

#include "Base.h"
#include "Ant.h"
#include "Object.h"
#include "MeleeAnt.h"
#include "RangedAnt.h"
#include "TankAnt.h"
#include "Level.h"

#include "Point.h"

#include "Money.h"
#include "MoneyNeeded.h"

#include "Command.h"

#include <string>

#include <unordered_map>
#include <map>
#include <memory>

#include <cstdint>
#include <iostream>

class Player
{
protected:

	// selected base uniq id
	// used for training the troops to one selected base based on this uniq id
	uint32_t selectedBase;
	
	// saving all the bases of this player
	std::unordered_map<uint32_t, std::shared_ptr<Object>> bases;

	// saving all the objects of this particular player
	std::unordered_map<uint32_t, std::shared_ptr<Object>> stuff;

	// saving the in range objects
	std::unordered_map<uint32_t, std::map<double, std::shared_ptr<Object>>> range;
	// saving the closest array of object to certain id of object
	std::unordered_map<uint32_t, std::map<double, std::shared_ptr<Object>>> closest;
	
	// all the progressing info
	// level, money, troops costs and training time

	Level points;

	Money golds;

	MoneyNeeded costs;

	// todo
	// training time class
	//	-> derived of MoneyNeeded

public:

	// cons/destructor

	Player();

	Player(const Level& points, const Money& rewards);

	~Player();

	// operators

	friend std::ostream& operator << (std::ostream& os, const Player& x);

	// methods

	/**
	 * Finds and fills the data structures with according enemy objects.
	 * 
	 * \param enemy player
	 */
	void FindEnemy(Player& enemy);

	/**
	 * Commands from players here.
	 * training ants, building bases etc.
	 * 
	 * \param cmd string command to be executed
	 */
	virtual void Input(const std::string& cmd);

	/**
	 * Commands from players here.
	 * training ants, building bases etc.
	 *
	 * \param cmd command of type Command to be executed
	 */
	virtual void Input(Command& cmd);

	/**
	 * Add object to this player.
	 * 
	 * \param obj to be added inside shared_ptr
	 */
	void Add(std::shared_ptr<Object> obj);

	/**
	 * Auxiliary method to print contents of all three data structures used to save objects.
	 * 
	 */
	void PrintOut();

	/**
	 * ALl the action all objects has to do.
	 * 
	 */
	void Actions();

	/**
	 * Checks all the dead objects.
	 * 
	 */
	void CheckDead();

	// getters

	/**
	 * Golds getter.
	 * 
	 * \return golds of object Money
	 */
	Money& GetGolds();
	
	/**
	 * Level getter.
	 * 
	 * \return points of object Level
	 */
	Level& GetLevel();

	/**
	 * Costs getter.
	 * 
	 * \return costs of object MoneyNeeded
	 */
	MoneyNeeded& GetCosts();

	/**
	 * Selected base getter.
	 * 
	 * \return reference to selected base
	 */
	Base& GetSelected();

};
