#include "Player.h"

Player::Player(const std::string& name)
	:golds(0),
	selectedBase(-1),
	name(name)
{}

Player::Player(const Level& points, const Money& rewards, const std::string& name)
	:points(points),
	golds(golds),
	selectedBase(-1),
	name(name)
{}

Player::~Player()
{}

std::ostream& operator<<(std::ostream& os, Player& obj)
{
	os << obj.name << std::endl;
	for (auto& x : obj.stuff) {
		os << ":" << *x.second << std::endl;

		if (x.second->type != Base::baseType)
			continue;
		if (obj.train[x.first].empty()) {
			os << "-" << std::endl << std::endl;
			continue;
		}
		os << '+' << std::endl;

		for (auto& y : obj.train[x.first])
			os << ":" << *y << std::endl;
		os << std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, Player& obj)
{
	int t;
	int x, y;
	double h;

	char ch;

	if (!(is >> obj.name))
		throw std::invalid_argument("Cant read name");

	while (is.good()) {

		is.get();
		ch = is.get();
		if (ch != ':')
			break;

		if (!(is >> t >> x >> y >> h))
			throw std::invalid_argument("Cant read data, corrupted");

		std::shared_ptr<Object> temp;
		if (t == MeleeAnt::meleeAntCode.code)
			temp = std::make_shared<MeleeAnt>(MeleeAnt());
		else if (t == RangedAnt::rangedAntCode.code)
			temp = std::make_shared<RangedAnt>(RangedAnt());
		else if (t == TankAnt::tankAntCode.code)
			temp = std::make_shared<TankAnt>(TankAnt());
		else if (t == Base::baseType.code)
			temp = std::make_shared<Base>(Base());
		else
			throw std::invalid_argument("Wrong type");

		temp->Set(Point(x, y), h);

		obj.stuff[temp->GetId()] = temp;

		if (temp->type != Base::baseType)
			continue;

		is.get();
		ch = is.get();
		if (ch == '-')
			continue;

		while (is.good()) {
			ch = is.get();

			if (ch != ':')
				break;
			if (!(is >> t >> x >> y >> h))
				throw std::invalid_argument("Cant read data, corrupted");

			std::shared_ptr<Object> in;
			if (t == MeleeAnt::meleeAntCode.code)
				in = std::make_shared<MeleeAnt>(MeleeAnt());
			else if (t == RangedAnt::rangedAntCode.code)
				in = std::make_shared<RangedAnt>(RangedAnt());
			else if (t == TankAnt::tankAntCode.code)
				in = std::make_shared<TankAnt>(TankAnt());
			else
				throw std::invalid_argument("Wrong type");

			in->Set(Point(x, y), h);

			obj.train[temp->GetId()].push_back(in);
		}
	}

	return is;
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

void Player::Input(Command& cmd)
{
	if (cmd == Command::nextBase || cmd == Command::backBase) {
		auto it = bases.find(selectedBase);
		if (it == bases.end()) {
			it = bases.begin();
			selectedBase = it->second->GetId();
			return;
		}
		if (cmd == Command::nextBase)
			it++;
		else
			it--;
		selectedBase = it->second->GetId();
	}
	else if (cmd == Command::trainMelee
		|| cmd == Command::trainRange
		|| cmd == Command::trainTank)
	{
		if (bases.find(selectedBase) == bases.end())
			throw std::invalid_argument("Not selected base where to train");
		if (train[selectedBase].size() == 5)
			throw std::invalid_argument("Training queue is full");

		if (train[selectedBase].empty())
			ticking[selectedBase] = std::chrono::steady_clock::now();

		if (cmd == Command::trainMelee
			&& golds.GetMoney() >= costs[MeleeAnt::meleeAntCode])
		{
			train[selectedBase].push_back(std::make_shared<MeleeAnt>(MeleeAnt(Point(bases[selectedBase]->GetCoords()))));
		}
		else if (cmd == Command::trainRange
			&& golds.GetMoney() >= costs[RangedAnt::rangedAntCode])
		{
			train[selectedBase].push_back(std::make_shared<RangedAnt>(RangedAnt(Point(bases[selectedBase]->GetCoords()))));
		}
		else if (cmd == Command::trainTank
			&& golds.GetMoney() >= costs[TankAnt::tankAntCode])
		{
			train[selectedBase].push_back(std::make_shared<TankAnt>(TankAnt(Point(bases[selectedBase]->GetCoords()))));
		}

	}
	else {
		throw std::invalid_argument("Unknown command");
	}
}

void Player::Add(std::shared_ptr<Object> obj)
{
	stuff[obj->GetId()] = obj;
	if (obj->type == Base::baseType) {
		bases[obj->GetId()] = obj;
		train[obj->GetId()] = {};
	}
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

	auto it = stuff.begin();
	while(it != stuff.end()) {

		auto ptr = it->second;
		it++;

		if (ptr->IsAlive())
			continue;

		if (ptr->type == Base::baseType) {
			bases.erase(ptr->GetId());
			if (selectedBase == ptr->GetId())
				selectedBase = -1;
			
			ticking.erase(ptr->GetId());
			train.erase(ptr->GetId());
		}

		stuff.erase(ptr->GetId());
	}

	int temp = points.CheckLevel();
	for (int i = 0; i < temp; i++) {
		golds.Up();
		costs.Up();
		points.Up();

		// todo
		// need to raise the stats of objects
	}

}

void Player::CheckTrain()
{
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> diff;

	for (auto& x : train) {
		if (train[x.first].empty())
			continue;

		diff = end - ticking[x.first];

		std::shared_ptr<Object> temp = train[x.first].front();

		if (diff.count() < (double)times[*temp] / 1000)
			continue;

		Add(temp);
		train[x.first].pop_front();
		ticking[x.first] = end;
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
	if(bases.find(selectedBase) == bases.end())
		throw std::invalid_argument("Not selected base");

	return (Base&)(*bases[selectedBase]);
}

std::list<std::shared_ptr<Object>>& Player::GetTrain()
{
	if (bases.find(selectedBase) == bases.end())
		throw std::invalid_argument("Not selected base");

	return train[selectedBase];
}

std::chrono::steady_clock::time_point& Player::GetTicking()
{
	if (bases.find(selectedBase) == bases.end())
		throw std::invalid_argument("Not selected base");

	return ticking[selectedBase];
}
