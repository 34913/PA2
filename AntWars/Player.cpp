#include "Player.h"

Player::Player()
	:golds(0)
{}

Player::Player(const Level& points, const Money& rewards)
	:points(points),
	golds(golds)
{}

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

void Player::Input(const std::string& cmd)
{
	// todo
	// setting one base
	// train ants and certain types of ants based on commands

}

void Player::Input(Command& cmd)
{
	if (cmd == Command::nextBase) {

	}
	else if (cmd == Command::backBase) {

	}
	else if (cmd == Command::trainMelee) {

	}
	else if (cmd == Command::trainRange) {

	}
	else if (cmd == Command::trainTank) {

	}
	else {
		throw std::invalid_argument("Not known command");
	}
}

void Player::Add(std::shared_ptr<Object> obj)
{
	stuff[obj->GetId()] = obj;
	if (obj->type == Base::baseType)
		bases[obj->GetId()] = obj;
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

	// for cycle
	auto itStuff = stuff.begin();
	// for changing to either to move to closest or attack on someone in range
	auto maps = &range;

	while (itStuff != stuff.end()) {
		// if is alive, then find enemy
		if (!itStuff->second->IsAlive()) {
			itStuff++;
			continue;
		}
		auto it = (*maps)[itStuff->first].begin();

		// determine who is the target (move/attack)
		while (it != (*maps)[itStuff->first].end()) {
			if (it->second->IsAlive())
				break;
			it++;
		}

		// if some target is found
		if (it != (*maps)[itStuff->first].end()) {
			// attacking part
			if (maps == &range) {
				(*itStuff->second).Attack(*it->second);
				if (!it->second->IsAlive()) {

					// adding money and points if killed
					//	-> ergo is dead
					//		-> ergo is not alive

					golds.AddMoney(*it->second);

					points.AddExp(*it->second);
				}
			}
			// moving part
			else {
				((Ant&)(*itStuff->second)).Move(it->second->GetCoords());
				maps = &range;
			}
		}
		// switch the polarity
		// -> if no target to attack in range, then move to closest
		// (need to work on that one too, to not stuck them on one place)
		else {
			if (maps == &range && itStuff->second->type != Base::baseType) {
				maps = &closest;
				continue;
			}
			else
				maps = &range;
		}

		// next only if checked range (and attacked)
		// or also checked closest, only then move on
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

	int temp = points.CheckLevel();
	for (int i = 0; i < temp; i++) {
		golds.Up();
		costs.Up();
		points.Up();
	}

}

Money& Player::GetGolds()
{
	return golds;
}

Level& Player::GetLevel()
{
	return points;
}

MoneyNeeded& Player::GetCosts()
{
	return costs;
}

Base& Player::GetSelected()
{
	return (Base&)(*bases[selectedBase]);
}
