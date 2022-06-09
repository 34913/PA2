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
	:name(name)
{
	points.begin();
	golds.begin();
	costs.begin();
	times.begin();
}

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
		os << *x.second.ptr << std::endl;

		if (x.second.ptr->type != Base::type)
			continue;
		if (obj.bases[x.first].train.empty()) {
			os << "-" << std::endl << std::endl;
			continue;
		}

		os << '+' << obj.bases[x.first].train.size() << std::endl;
		for (auto& y : obj.bases[x.first].train)
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

		obj.stuff[temp->GetId()].ptr = temp;
		obj.stuff[temp->GetId()].range = {};
		obj.stuff[temp->GetId()].closest = {};

		if (temp->type != Base::type)
			continue;

		obj.bases[temp->GetId()].ptr = temp;
		obj.bases[temp->GetId()].train = {};
		obj.bases[temp->GetId()].ticking = {};

		// if base
		// read all the training ants in queue
		is.get();
		ch = is.get();
		if (ch == '-')
			continue;
		is >> trainCount;

		obj.bases[temp->GetId()].ticking = std::chrono::steady_clock::now();
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

			obj.bases[temp->GetId()].train.push_back(temp);
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

			// basicaly first try if the target is in range
			// and add him in std::set in range based on the length from it
			if (x.second.ptr->HasInRange(*enemyX.second.ptr, len))
				x.second.range[len] = enemyX.second.ptr;
			// if not, add him (again based on length) in std::set in closest
			else if (x.second.ptr->type != Base::type)
				x.second.closest[len] = enemyX.second.ptr;

		}
	}
}

void Player::Input(Command& cmd)
{
	// shifting the selected base
	if (cmd == Command::nextBase || cmd == Command::backBase) {
		if (bases.empty())
			throw std::invalid_argument("No bases for " + name);
		auto it = bases.find(selectedBase);
		if (it == bases.end()) {
			it = bases.begin();
			selectedBase = it->second.ptr->GetId();
			return;
		}
		if (cmd == Command::nextBase) {
			it++;
			if (it == bases.end())
				it = bases.begin();
		}
		else {
			return;

			// not possible
			// can cycle all and set the previous, not worth it really
			
			//if (it == bases.begin())
			//	it = bases.end();
			//it--;
		}
		selectedBase = it->second.ptr->GetId();
	}
	// training troops in selected base
	else if (cmd == Command::trainMelee
		|| cmd == Command::trainRange
		|| cmd == Command::trainTank)
	{
		auto it = bases.find(selectedBase);

		// exceptions if not selected base
		if (it == bases.end())
			throw std::invalid_argument("Not selected base where to train");
		// full queue (just named like that, its list but i thought training queue
		// list easier to implement

		if (it->second.train.size() == 5)
			throw std::invalid_argument("Training queue is full");

		// setting the start of training time if empty
		//	-> need to start it first
		if (it->second.train.empty())
			it->second.ticking = std::chrono::steady_clock::now();

		// choosing the right to train
		std::shared_ptr<Object> temp;
		if (cmd == Command::trainMelee && golds.GetMoney() >= costs[MeleeAnt::type])
			temp = std::make_shared<MeleeAnt>(MeleeAnt(Point(bases[selectedBase].ptr->GetCoords())));
		else if (cmd == Command::trainRange && golds.GetMoney() >= costs[RangedAnt::type])
			temp = std::make_shared<RangedAnt>(RangedAnt(Point(bases[selectedBase].ptr->GetCoords())));
		else if (golds.GetMoney() >= costs[TankAnt::type])
			temp = std::make_shared<TankAnt>(TankAnt(Point(bases[selectedBase].ptr->GetCoords())));
		else
			throw std::invalid_argument("Not enough money");

		bases[selectedBase].train.push_back(temp);

		golds.RemoveMoney(costs[temp->type.code]);
	}
}

void Player::Add(std::shared_ptr<Object> obj)
{

	stuff[obj->GetId()].ptr = obj;
	stuff[obj->GetId()].closest = {};
	stuff[obj->GetId()].range = {};

	if (obj->type != Base::type)
		return;

	bases[obj->GetId()].ptr = obj;
	bases[obj->GetId()].train = {};
	bases[obj->GetId()].ticking = {};
	
}

void Player::PrintOut()
{
	for (auto& x : stuff) {
		std::cout << *x.second.ptr << std::endl;

		std::cout << "range:" << std::endl;
		for (auto& a : x.second.range)
			std::cout << "   " << a.first << " - " << *a.second << std::endl;

		std::cout << "closest:" << std::endl;
		for (auto& a : x.second.closest)
			std::cout << "   " << a.first << " - " << *a.second << std::endl;
	}
	std::cout << std::endl << std::endl;
}

void Player::Actions(Map& show)
{
	//PrintOut();

	// for cycle
	auto itStuff = stuff.begin();
	if (itStuff == stuff.end())
		return;

	// for changing to either to move to closest or attack on someone in range
	std::map<double, std::shared_ptr<Object>>* maps = &itStuff->second.range;

	Point go(0, 0);

	while (true) {
		// if is alive, then find enemy
		if (!itStuff->second.ptr->IsAlive()) {
			itStuff++;
			if (itStuff == stuff.end())
				break;
			maps = &itStuff->second.range;
			continue;
		}
		auto it = maps->begin();

		// determine who is the target (move/attack)
		while (it != maps->end()) {
			if (it->second->IsAlive())
				break;
			it++;
		}

		// if some target is found
		if (it != maps->end()) {
			// attacking part
			if (maps == &itStuff->second.range) {
				(*itStuff->second.ptr).Attack(*it->second);
				if (!it->second->IsAlive()) {

					// adding money and points if killed
					//	-> ergo is dead
					//		-> ergo is not alive

					golds.AddMoney(*it->second);

					points.AddExp(*it->second);
				}
				itStuff++;
				if (itStuff == stuff.end())
					break;
				maps = &itStuff->second.range;
			}
			// moving part
			else {
				// might want to change this
				// to move around the obstacle

				if (itStuff->second.ptr->type == Base::type)
					throw std::invalid_argument("Base cant move, internal error");

				Point pos = itStuff->second.ptr->GetCoords();

				// try moving in desired way
				((Ant&)(*itStuff->second.ptr)).TryMove(it->second->GetCoords(), go);

				// check, if its clear, go that way
				if (show[go.y][go.x] == Map::EMPTY) {
					show[pos.y][pos.x] = Map::EMPTY;
					((Ant&)(*itStuff->second.ptr)).Move(go);
					show[go.y][go.x] = Map::ANT_P1;
				}
				else {

					// decides where are u headed -> direction
					//	-> gets it in form of int index -> Point::dirs array can be accessed
					int dir = itStuff->second.ptr->GetCoords().FindDir(go);

					int cA = 0;
					int cB = 0;

					// randomly decides, the ants might get stuck, it is not a bug, it is a feature
					// ants try their way more or less in mass randomly, to get to the finish
					// trying "randomly" to choose a way, first the way which is closer in turning as my desired way
					//	-> and so on
					while (cA != 5 && cB != 4) {

						bool r = rand() % 2;
						if ((r && (cA < 4 || cB == 4)) || (cA == 4 && cB == 4)) {
							cA++;
							go = itStuff->second.ptr->GetCoords() + Point::GetDir(dir - cA);
						}
						else {
							cB++;
							go = itStuff->second.ptr->GetCoords() + Point::GetDir(dir + cB);
						}

						if (show[go.y][go.x] == Map::EMPTY) {
							show[pos.y][pos.x] = Map::EMPTY;
							((Ant&)(*itStuff->second.ptr)).Move(go);
							show[go.y][go.x] = Map::ANT_P1;
							break;
						}
					}
				}

				itStuff++;
				if (itStuff == stuff.end())
					break;
				maps = &itStuff->second.range;
			}
		}
		// switch the polarity
		// -> if no target to attack in range, then move to closest
		// (need to work on that one too, to not stuck them on one place)
		else {
			if (maps == &itStuff->second.range && itStuff->second.ptr->type != Base::type) {
				maps = &itStuff->second.closest;
			}
			else {
				itStuff++;
				if (itStuff == stuff.end())
					break;
				maps = &itStuff->second.range;
			}
		}
	}

}

void Player::CheckDead()
{
	auto it = stuff.begin();
	while(it != stuff.end()) {

		it->second.closest.clear();
		it->second.range.clear();

		auto& ptr = it->second.ptr;
		it++;

		if (ptr->IsAlive())
			continue;

		if (ptr->type == Base::type) {
			bases.erase(ptr->GetId());
			if (selectedBase == ptr->GetId())
				selectedBase = -1;
		}

		stuff.erase(ptr->GetId());
	}

	int temp = points.CheckLevel();
	for (int i = 0; i < temp; i++) {
		golds.Up();
		costs.Up();
		points.Up();

		points.Up();
	}

}

void Player::CheckTrain()
{
	auto end = std::chrono::steady_clock::now();
	std::chrono::milliseconds millis;

	for (auto& x : bases) {
		if (x.second.train.empty())
			continue;

		std::shared_ptr<Object> temp = x.second.train.front();

		millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - x.second.ticking);
		if (millis.count() < times[*temp])
			continue;

		Add(temp);
		x.second.train.pop_front();
		x.second.ticking = end;
	}
}

void Player::StopTrain()
{
	lastTime = std::chrono::steady_clock::now();
}

void Player::ResumeTrain()
{
	auto temp = std::chrono::steady_clock::now();
	for (auto& x : bases)
		x.second.ticking = temp - lastTime + x.second.ticking;
}

bool Player::CheckBases()
{
	return bases.empty();
}

const std::string& Player::GetName()
{
	return name;
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

TrainingTime& Player::GetTimes()
{
	return times;
}

bool Player::IsSelected()
{
	return bases.find(selectedBase) != bases.end();
}

Base& Player::GetSelected()
{
	if(bases.find(selectedBase) == bases.end())
		throw std::invalid_argument("Not selected base");

	return (Base&)(*bases[selectedBase].ptr);
}

BaseType& Player::GetBase(uint32_t id)
{
	auto it = bases.find(id);
	if (it == bases.end())
		throw std::invalid_argument("Not found in bases");
	return it->second;
}

std::unordered_map<uint32_t, BaseType>& Player::GetBase()
{
	return bases;
}

StuffType& Player::GetStuff(uint32_t id)
{
	auto it = stuff.find(id);
	if (it == stuff.end())
		throw std::invalid_argument("Not found in stuff");
	return it->second;
}

std::unordered_map<uint32_t, StuffType>& Player::GetStuff()
{
	return stuff;
}

std::chrono::steady_clock::time_point& Player::GetLastTime()
{
	return lastTime;
}
