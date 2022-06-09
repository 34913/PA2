#include "Game.h"

bool Game::CheckMap()
{
	// cant stand on each other or in the wall
	//	-> or ouside ofc
	// cant be stucked in base, no problem with that, but it must be their base, not in enemy

	for (auto& x : p1.GetStuff()) {
		Point& temp = x.second.ptr->GetCoords();

		if (temp.x < 0 || temp.x >= show.GetWidth()
			|| temp.y < 0 || temp.y >= show.GetHeight())
			return true;

		if (show[temp.y][temp.x] != Map::EMPTY)
			return true;

		for (auto& y : p2.GetBase()) {
			if (y.second.ptr->GetCoords() == temp)
				return true;
		}
	}

	for (auto& x : p2.GetStuff()) {
		Point& temp = x.second.ptr->GetCoords();

		if (temp.x < 0 || temp.x > show.GetWidth()
			|| temp.y < 0 || temp.y > show.GetHeight())
			return true;

		if (show[temp.y][temp.x] != Map::EMPTY)
			return true;

		for (auto& y : p1.GetBase()) {
			if (y.second.ptr->GetCoords() == temp)
				return true;
		}
	}
	return false;
}

Game::Game(std::string dir, int pause)
	:pause(pause),
	low(pause / 10),
	high(pause * 10),
	rise(pause / 10),
	dir(dir)
{}

int Game::Load(std::string saveName)
{
	std::ifstream myFileI;
	std::string path;

	loaded = false;

#ifdef _WIN32
	path = dir + "\\" + saveName + "\\file.txt";
#else
	path = dir + "/" + saveName + "/file.txt";
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
		throw e;
	}
	myFileI.close();

#ifdef _WIN32
	path = dir + "\\" + saveName + "\\map.txt";
#else
	path = dir + "/" + saveName + "/map.txt";
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
		throw e;
	}
	myFileI.close();

	if (CheckMap()) {
		throw std::invalid_argument("This save cant be used, the players and map does not comply, must have been modified");
	}

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
	std::string dirSave = dir + "\\" + saveName;
	pathFile = dirSave + "\\file.txt";
	pathMap = dirSave + "\\map.txt";
#else
	std::string dirSave = dir + "/" + saveName;
	pathFile = dirSave + "/file.txt";
	pathMap = dirSave + "/map.txt";
#endif
	
	// chceck if it already exists
	myFileI.open(pathFile);
	if (myFileI.is_open())
		throw std::invalid_argument("Save already exists with name " + dir);
	myFileI.open(pathMap);
	if (myFileI.is_open())
		throw std::invalid_argument("Save already exists with name " + dir);

	if (!std::filesystem::create_directory(std::filesystem::path(dirSave)))
		throw std::invalid_argument("Cant create directory " + dirSave);

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

	return p1.CheckBases() || p2.CheckBases();
}

bool Game::Check()
{
	return !p1.CheckBases() && !p2.CheckBases();
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
