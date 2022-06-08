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
#include <filesystem>
#include <fstream>

class Game
{
	// true if game has been succesfully loaded
	bool loaded = false;

	// true if game is running
	bool running = false;

	// settings of how fast is the game running, rise the speed etc..
	//	-> everything in milliseconds
	int pause;
	const int low;
	const int high;
	const int rise;
	std::string dir;

	/**
	 * Checks if the map is okay with ants and bases in position getted from file.
	 * error is
	 *	-> being ouside the bounds of playing field
	 *	-> being inside wall
	 *	-> being inside Ant
	 *	-> being inside enemy Base, yours can be, as it is in real live
	 * 
	 * \return true if error occurred
	 */
	bool CheckMap();

public:

	enum cmd {
		stop = false,
		play = true,
	};

	// accesible
	// players and map

	Map show;

	Player p1;
	AutoPlayer p2;

	// constructor

	Game(std::string dir = "examples", int pause = 500);

	// methods
	
	/**
	 * Loads all the assets from given directory.
	 *
	 * \param directory where to load it from (relative path)
	 * \return 0 on success
	 */
	int Load(std::string saveName);

	/**
	 * Saves ongoing game in given directory.
	 * 
	 * \param directory where to save it (relative path)
	 * \return 0 on success
	 */
	int Save(std::string saveName);

	/**
	 * Plays one cycle of game.
	 * 
	 * \return 0 if game is still runnning
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
	 * Resumes/Pauses the game.
	 * 
	 * \param command of type cmd, if equals stop, game will be stopped
	 */
	void RunStop(cmd command);

	/**
	 * Increases speed of the game.
	 * 
	 */
	void Faster();

	/**
	 * Decreases speed of the game.
	 */
	void Slower();

	// getters

	bool GetRunning();

	bool GetLoaded();

	int GetPause();

	std::string& GetDir();

	void GetFiles(std::vector<std::string>& arr);

};

#endif
