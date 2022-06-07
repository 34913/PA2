#ifndef Game_H
#define Game_H

// #include "Object.h"
#include "TypeCode.h"

#include "Ant.h"
#include "MeleeAnt.h"
#include "RangedAnt.h"
#include "TankAnt.h"

#include "Player.h"
#include "AutoPlayer.h"
#include "Map.h"

#include "Base.h"

#include <iostream>

class Game
{
	// true if game is running
	bool running;

	// settings of how fast is the game running, rise the speed etc..
	//	-> everything in milliseconds
	int pause;
	const int low;
	const int high;
	const int rise;

public:

	// accesible
	// players and map

	Map show;

	Player p1;
	AutoPlayer p2;

	// constructor

	Game(int pause = 500);

	// methods

	/**
	 * Loads all the assets.
	 * 
	 * \return 
	 */
	int Load();

	/**
	 * Plays one cycle of game.
	 * 
	 */
	int Play();

	/**
	 * Checks if both sides has any bases left.
	 * 
	 * \return true if both players have any bases
	 */
	bool Check();

	/**
	 * Resumes/Pauses the game.
	 *	-> inverts the runnning value
	 * 
	 */
	void RunStop();

	/**
	 * Increases speed of the game.
	 */
	void Faster();

	/**
	 * Decreases speed of the game.
	 */
	void Slower();

	// getters

	bool GetRunning();

	int GetPause();

};

#endif
