#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	stuff.clear();
	range.clear();
	closest.clear();
}

std::ostream& operator<<(std::ostream& os, const Player& x)
{
	for (auto& x : x.stuff)
		std::cout << ":" << *x.second << std::endl;
	return os;
}

void Player::FindEnemy(Player& enemy)
{
	double len = 1;
	for (auto& x : stuff) {
		for (auto& enemyX : enemy.stuff) {

			if (x.second->HasInRange(*enemyX.second, len))
				range[x.first][len] = enemyX.second;
			else if(x.second->type != Base::baseType)
				closest[x.first][len] = enemyX.second;

		}
	}
}

void Player::Command(const std::string& cmd)
{
	// todo
	// setting one base
	// train ants and certain types of ants based on commands
	
	


}

void Player::Add(std::shared_ptr<Object> obj)
{
	stuff[obj->GetId()] = obj;
}

void Player::PrintOut()
{
	for (auto& x : stuff) {
		std::cout << *stuff[x.first] << std::endl;

		std::cout << "range:" << std::endl;
		for (auto& a : range[x.first])
			std::cout << "   " << a.first << " - " << *a.second << std::endl;

		std::cout << "closest:" << std::endl;
		for (auto& a : closest[x.first])
			std::cout << "   " << a.first << " - " << *a.second << std::endl;
	}
	std::cout << std::endl << std::endl;
}

void Player::Actions()
{
	//PrintOut();

	auto itStuff = stuff.begin();
	std::unordered_map<uint32_t, std::map<double, std::shared_ptr<Object>>>* maps = &range;

	while (itStuff != stuff.end()) {
		auto it = (*maps)[itStuff->first].begin();
		//std::shared_ptr<Object> temp = NULL;

		while (it != (*maps)[itStuff->first].end()) {
			if (it->second->IsAlive())
				//temp = it->second;
				break;
			it++;
		}

		if (it != (*maps)[itStuff->first].end()) {
			if (maps == &range) {
				(*itStuff->second).Attack(*it->second);
				if (!it->second->IsAlive()) {
					// add golds for killing the object
				}
			}
			else {
				((Ant&)(*itStuff->second)).Move(it->second->GetCoords());
				maps = &range;
			}
		}
		else {
			if (maps == &range && itStuff->second->type != Base::baseType) {
				maps = &closest;
				continue;
			}
			else
				maps = &range;
		}
		itStuff++;
	}

}

void Player::CheckDead()
{
	range.clear();
	closest.clear();

	for (auto& x : stuff) {
		if (!x.second->IsAlive())
			stuff.erase(x.first);
	}
}
