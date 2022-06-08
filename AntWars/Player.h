#ifndef Player_H
#define Player_H

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

#include "Map.h"

#include <string>

#include <unordered_map>
#include <map>
#include <list>
#include <memory>

#include <cstdint>
#include <iostream>
#include <chrono>

#include <fstream>

/**
 * Saving all the data about base.
 * ptr, training queue and time point when the training begun
 */
typedef struct BaseStruct
{

	std::shared_ptr<Object> ptr;
	std::list<std::shared_ptr<Object>> train;
	std::chrono::steady_clock::time_point ticking;

} BaseType;

/**
 * Saving all the data about any Stuff.
 * like ptr, closest or in range to be used afterwards
 */
typedef struct StuffStruct
{

	std::shared_ptr<Object> ptr;
	std::map<double, std::shared_ptr<Object>> range;
	std::map<double, std::shared_ptr<Object>> closest;

} StuffType;

class Player
{
protected:

	// name of player, implicitly Player in constructor
	std::string name;

	// saving all the objects under player
	std::unordered_map<uint32_t, BaseType> bases;
	std::unordered_map<uint32_t, StuffType> stuff;

	// selected base uniq id
	// used for training the troops to one selected base based on this uniq id
	uint32_t selectedBase = -1;
	
	// all the progressing info
	// level, money, troops costs and training time

	Level points;

	Money golds;

	MoneyNeeded costs;

	TrainingTime times;

	/**
	 * Creates new object with shared_ptr based on type without any info so far.
	 * 
	 * \param type determines which object of which type it is
	 * \return the shared_ptr to created object
	 */
	std::shared_ptr<Object> Create(int type);

	// saves the time in case of pause of the game
	// needs to be done, otherwise trains without control on stop of the game
	std::chrono::steady_clock::time_point time;

public:

	// cons/destructor

	Player(const std::string& name = "Player");
	
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
	void Actions(Map& show);

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

	/**
	 * Stop the training gueue, so save the info about it.
	 * It must be restored afterwards
	 * 
	 */
	void StopTrain();

	/**
	 * Restores training again.
	 * 
	 */
	void ResumeTrain();

	/**
	 * Returns info about players bases.
	 * 
	 * \return true if player does not have any bases left
	 */
	bool CheckBases();

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

	BaseType& GetBase(uint32_t id);

	std::unordered_map<uint32_t, BaseType>& GetBase();

	StuffType& GetStuff(uint32_t id);

	std::unordered_map<uint32_t, StuffType>& GetStuff();

};

#endif
