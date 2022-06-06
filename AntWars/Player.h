#pragma once

#include "Base.h"
#include "Object.h"
#include "MeleeAnt.h"
#include "RangedAnt.h"
#include "TankAnt.h"

#include "Point.h"

#include "MoneyNeeded.h"
#include "Level.h"
#include "Money.h"
#include "TrainingTime.h"

#include "Command.h"

#include <string>

#include <unordered_map>
#include <map>
#include <list>
#include <memory>

#include <cstdint>
#include <iostream>
#include <chrono>

#include <fstream>

class Player
{
protected:

	std::string name;

	// selected base uniq id
	// used for training the troops to one selected base based on this uniq id
	uint32_t selectedBase;
	
	// saving all the bases of this player
	std::map<uint32_t, std::shared_ptr<Object>> bases;

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

	TrainingTime times;

	std::unordered_map<uint32_t, std::list<std::shared_ptr<Object>>> train;
	std::unordered_map<uint32_t, std::chrono::steady_clock::time_point> ticking;

	std::shared_ptr<Object> Create(int type);

public:

	// cons/destructor

	Player(const std::string& name = "Player");
	
	Player(const Level& points, const Money& rewards, const std::string& name = "Player");

	~Player();

	// operators

	friend std::ostream& operator << (std::ostream& os, Player& obj);

	friend std::istream& operator >> (std::istream& is, Player& obj);

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

	/**
	 * Checks if some troop is already trained, then added in stuff.
	 * 
	 */
	void CheckTrain();

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

	/**
	 * Seleected base training list getter.
	 *
	 * \return reference to this list
	 */
	std::list<std::shared_ptr<Object>>& GetTrain();

	/**
	 * Ticking time of selected base getter
	 *
	 * \return reference to this time point
	 */
	std::chrono::steady_clock::time_point& GetTicking();

};
