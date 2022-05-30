#pragma once

#include "Player.h"

class AutoPlayer: public Player
{

public:
	
	AutoPlayer(const std::string& name = "Automat");

	void Input();

};
