#include "Player.h"

std::shared_ptr<Object> Player::Create(int type)
{
	std::shared_ptr<Object> temp;
	if (type == MeleeAnt::type.code)
		temp = std::make_shared<MeleeAnt>(MeleeAnt());
	else if (type == RangedAnt::type.code)
		temp = std::make_shared<RangedAnt>(RangedAnt());
	else if (type == TankAnt::type.code)
		temp = std::make_shared<TankAnt>(TankAnt());
	else if (type == Base::type.code)
		temp = std::make_shared<Base>(Base());
	else
		throw std::invalid_argument("Wrong type of obj");

	return temp;
}

Player::Player(const std::string& name)
	:golds(0),
	selectedBase(-1),
	name(name)
{
	points.begin();
	golds.begin();
	costs.begin();
	times.begin();
}

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

	os << obj.points << std::endl;
	os << obj.golds << std::endl;
	os << obj.costs << std::endl;
	os << obj.times << std::endl;

	os << obj.stuff.size() << std::endl;
	for (auto& x : obj.stuff) {
		os << *x.second << std::endl;

		if (x.second->type != Base::type)
			continue;
		if (obj.train[x.first].empty()) {
			os << "-" << std::endl << std::endl;
			continue;
		}

		os << '+' << obj.train[x.first].size() << std::endl;
		for (auto& y : obj.train[x.first])
			os << *y << std::endl;
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

	try {
		is >> obj.points;
		is >> obj.golds;
		is >> obj.costs;
		is >> obj.times;
	}
	catch (const std::invalid_argument& e) {
		throw e;
	}
	int count, trainCount;
	is >> count;

	for(int i = 0; i < count; i++) {

		if (!(is >> t >> x >> y >> h))
			throw std::invalid_argument("Cant read data, corrupted");

		std::shared_ptr<Object> temp = obj.Create(t);
		temp->Set(Point(x, y), h);

		obj.stuff[temp->GetId()] = temp;

		if (temp->type != Base::type)
			continue;

		// if base
		// read all the training ants in queue
		is.get();
		ch = is.get();
		if (ch == '-')
			continue;
		is >> trainCount;

		obj.ticking[temp->GetId()] = std::chrono::steady_clock::now();
		for(int i = 0; i < trainCount; i++) {

			if (!(is >> t >> x >> y >> h))
				throw std::invalid_argument("Cant read data, corrupted");

			try {
				temp = obj.Create(t);
			}
			catch (const std::invalid_argument& e) {
				throw e;
			}
			temp->Set(Point(x, y), h);

			obj.train[temp->GetId()].push_back(temp);
		}
	}

	obj.CheckDead();
	obj.CheckTrain();

	return is;
}

void Player::FindEnemy(Player& enemy)
{
	double len = 1;
	for (auto& x : stuff) {
		for (auto& enemyX : enemy.stuff) {

			// basicly first try if the target is in range
			// and add him in std::set in range based on the length from it
			if (x.second->HasInRange(*enemyX.second, len))
				range[x.first][len] = enemyX.second;
			// if not, add him (again based on length) in std::set in closest
			else if(x.second->type != Base::type)
				closest[x.first][len] = enemyX.second;

		}
	}
}

void Player::Input(Command& cmd)
{
	// shifting the selected base
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
	// training troops in selected base
	else if (cmd == Command::trainMelee
		|| cmd == Command::trainRange
		|| cmd == Command::trainTank)
	{
		// exceptions if not selected base
		if (bases.find(selectedBase) == bases.end())
			throw std::invalid_argument("Not selected base where to train");
		// full queue (just named like that, its list but i thought training queue
		// list easier to implement
		if (train[selectedBase].size() == 5)
			throw std::invalid_argument("Training queue is full");

		// setting the start of trainíng time if empty
		//	-> need to start it first
		if (train[selectedBase].empty())
			ticking[selectedBase] = std::chrono::steady_clock::now();

		// choosing the right to train
		std::shared_ptr<Object> temp;
		if (cmd == Command::trainMelee && golds.GetMoney() >= costs[MeleeAnt::type])
		{
			temp = std::make_shared<MeleeAnt>(MeleeAnt(Point(bases[selectedBase]->GetCoords())));
		}
		else if (cmd == Command::trainRange && golds.GetMoney() >= costs[RangedAnt::type])
		{
			temp = std::make_shared<RangedAnt>(RangedAnt(Point(bases[selectedBase]->GetCoords())));
		}
		else if(golds.GetMoney() >= costs[TankAnt::type])
		{
			temp = std::make_shared<TankAnt>(TankAnt(Point(bases[selectedBase]->GetCoords())));
		}
		train[selectedBase].push_back(temp);

	}
	// unknown command
	else
		throw std::invalid_argument("Unknown command");
}

void Player::Add(std::shared_ptr<Object> obj)
{
	stuff[obj->GetId()] = obj;
	if (obj->type == Base::type) {
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
				// might want to change this
				// to move around the obstacle
				((Ant&)(*itStuff->second)).Move(it->second->GetCoords());
				maps = &range;
			}
		}
		// switch the polarity
		// -> if no target to attack in range, then move to closest
		// (need to work on that one too, to not stuck them on one place)
		else {
			if (maps == &range && itStuff->second->type != Base::type) {
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

		auto& ptr = it->second;
		it++;

		if (ptr->IsAlive())
			continue;

		if (ptr->type == Base::type) {
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
