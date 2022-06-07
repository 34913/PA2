#ifndef AutoPlayer_H
#define AutoPlayer_H

#include "Player.h"

class AutoPlayer: public Player
{
public:
	
	AutoPlayer(const std::string& name = "Automat");

	void Input();

};

#endif
