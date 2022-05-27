#pragma once

#include "Base.h"
#include "Ant.h"
#include "Object.h"
#include "MeleeAnt.h"
#include "RangedAnt.h"
#include "TankAnt.h"
#include "Level.h"

#include "Point.h"

//#include <vector>
#include <string>

#include <unordered_map>
#include <map>
#include <memory>

#include "Point.h"

#include <cstdint>
#include <iostream>

class Player
{
protected:

	// saving all the objects of this particular player
	std::unordered_map<uint32_t, std::shared_ptr<Object>> stuff;

	// maybe these two rework from multimap to map with vector inside to choose from
	// multiple closest opponents
	// now it is discarding multiple results with same double length, so remains just one
	// and there is no chance to choose afterwards

	// saving the in range objects
	std::unordered_map<uint32_t, std::map<double, std::shared_ptr<Object>>> range;
	// saving the closest array of object to certain id of object
	std::unordered_map<uint32_t, std::map<double, std::shared_ptr<Object>>> closest;
	
	Level points;

public:

	// cons/destructor

	Player();

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
	 * training ants, building bases
	 * 
	 * \param cmd command to be executed
	 */
	virtual void Command(const std::string& cmd);

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

};
