#include "Game.h"

Game::Game(std::string dir, int pause)
	:pause(pause),
	low(pause / 10),
	high(pause * 10),
	rise(pause / 10),
	dir(dir)
{}

int Game::Load(std::string saveName)
{
	//p1.Add(std::make_shared<RangedAnt>(RangedAnt(Point(57, 2))));
	//p1.Add(std::make_shared<Base>(Base(Point(2, 2))));

	//

	//p2.Add(std::make_shared<RangedAnt>(RangedAnt(Point(80, 70))));
	//p2.Add(std::make_shared<TankAnt>(TankAnt(Point(2, 47))));
	//p2.Add(std::make_shared<Base>(Base(Point(57, 47))));

	//p2.Add(TankAnt(Point(11, 12)));
	//p2.Add(TankAnt(Point(11, 10)));

	/*
	auto a = RangedAnt(Point(15, 15));
	auto b = RangedAnt(Point(15, 15));
	b.Attack(a);

	Object& temp = a;
	std::cout << typeid(temp).name() << '\n';

	auto smh = new RangedAnt();
	std::shared_ptr<Object&> neco;
	std::cout << typeid(*neco).name() << '\n';

	(*neco).Attack(b);

	//

	p1.Input(Command::nextBase);
	p1.Input(Command::trainMelee);
	*/

	std::ifstream myFileI;
	std::string path;

	loaded = false;

#ifdef _WIN32
	path = dir + "\\" + saveName + "\\file.txt";
#else
	path = directory + "/" + saveName + "/file.txt";
#endif
	// check if exists
	myFileI.open(path);
	if (!myFileI.is_open())
		throw std::invalid_argument("File " + path + " does not exist, save corrupted");

	// load players
	try {
		myFileI >> p1 >> p2;
	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	myFileI.close();

#ifdef _WIN32
	path = dir + "\\" + saveName + "\\map.txt";
#else
	path = directory + "/" + saveName + "/map.txt";
#endif
	// check if exists
	myFileI.open(path);
	if (!myFileI.is_open())
		throw std::invalid_argument("File " + path + " does not exist, save corrupted");

	// load map
	try {
		myFileI >> show;
	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		return -1;
	}
	myFileI.close();

	loaded = true;

	return loaded;
}

int Game::Save(std::string saveName)
{
	std::ifstream myFileI;
	std::ofstream myFileO;

	std::string pathFile;
	std::string pathMap;

	// set paths for files
#ifdef _WIN32
	pathFile = dir + "\\" + saveName + "\\file.txt";
	pathMap = dir + "\\" + saveName + "\\map.txt";
#else
	pathFile = directory + "/" + saveName + "/file.txt";
	pathMap = directory + "/" + saveName + "/map.txt";
#endif

	// chceck if it already exists
	myFileI.open(pathFile);
	if (myFileI.is_open())
		throw std::invalid_argument("Save already exists with name " + dir);
	myFileI.open(pathMap);
	if (myFileI.is_open())
		throw std::invalid_argument("Save already exists with name " + dir);

	myFileO.open(pathFile);
	if (!myFileO.is_open())
		throw std::invalid_argument("Cant open stream to save game");
		
	// upload players
	myFileO << p1 << p2;
	myFileO.close();

	myFileO.open(pathMap);
	if (!myFileO.is_open())
		throw std::invalid_argument("Cant open stream to save game");

	// upload map
	myFileO << show;
	myFileO.close();

	return 0;
}

int Game::Play()
{
	p1.FindEnemy(p2);
	p1.Actions(show);

	//

	p2.FindEnemy(p1);
	p2.Actions(show);

	//

	p1.CheckDead();
	p2.CheckDead();

	//

	p1.CheckTrain();
	p2.CheckTrain();

	//

	return p1.CheckBases() || p2.CheckBases();
}

bool Game::Check()
{
	return true;
}

void Game::RunStop()
{
	RunStop((cmd)(!running));
}

void Game::RunStop(cmd command)
{
	running = command;
	if (command) {
		p1.ResumeTrain();
		p2.ResumeTrain();
	}
	else {
		p1.StopTrain();
		p2.StopTrain();
	}
}

void Game::Faster()
{
	pause -= rise;
	if (pause < low)
		pause = low;
}

void Game::Slower()
{
	pause += rise;
	if (pause > high)
		pause = high;
}

bool Game::GetRunning()
{
	return running;
}

bool Game::GetLoaded()
{
	return loaded;
}

int Game::GetPause()
{
	return pause;
}

std::string& Game::GetDir()
{
	return dir;
}

void Game::GetFiles(std::vector<std::string>& arr)
{
	std::string temp;
	arr.clear();
	for (auto& x : std::filesystem::directory_iterator(dir)) {
		temp = x.path().string();

#ifdef _WIN32
		temp = temp.substr(temp.find("\\"));
#else
		temp = temp.substr(temp.find("/"));
#endif

		arr.push_back(temp);
	}
}
