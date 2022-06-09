#ifndef AutoPlayer_H
#define AutoPlayer_H

#include "Player.h"

class AutoPlayer: public Player
{
	int tankLimit = 10000;
	int rangedLimit = 5000;
	int meleeLimit = 500;

	std::chrono::steady_clock::time_point lastTank = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point lastRanged = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point lastMelee = std::chrono::steady_clock::now();

public:
	
	AutoPlayer(const std::string& name = "Automat");

	/**
	 * Sets the training time limits in milliseconds.
	 * 
	 * \param melee melee limit
	 * \param ranged ranged limit
	 * \param tank tank limit
	 */
	void SetLimits(int melee, int ranged, int tank);

	void Input();

};

#endif
