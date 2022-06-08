#include "Game.h"

Game::Game(int pause)
	:running(false),
	pause(pause),
	low(pause / 10),
	high(pause * 10),
	rise(pause / 10)
{}

int Game::Load()
{
	//p1.Add(std::make_shared<RangedAnt>(RangedAnt(Point(50, 50))));
	//p1.Add(std::make_shared<Base>(Base(Point(52, 50))));

	//

	//p2.Add(std::make_shared<RangedAnt>(RangedAnt(Point(80, 70))));
	//p2.Add(std::make_shared<TankAnt>(TankAnt(Point(30, 40))));
	//p2.Add(std::make_shared<Base>(Base(Point(60, 50))));

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

	std::ofstream myfile("file.txt");
	if (myfile.is_open()) {
		myfile << p1 << p2;
		myfile.flush();
	}
	*/

	std::ifstream myFileI;
	std::ofstream myFileO;

	myFileI.open("file.txt");
	if (myFileI.is_open()) {
		try {
			myFileI >> p1 >> p2;
		}
		catch (const std::invalid_argument& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	myFileI.close();

	myFileO.open("file.txt");
	if (myFileO.is_open())
		myFileO << p1 << p2;
	myFileO.close();

	myFileI.open("maps/mapA.txt");
	if (myFileI.is_open()) {
		try {
			myFileI >> show;
		}
		catch (const std::invalid_argument& e) {
			std::cout << e.what() << std::endl;
			return -1;
		}
	}
	myFileI.close();

	myFileO.open("maps/mapA.txt");
	if (myFileO.is_open())
		myFileO << show;
	myFileI.close();

	return 0;
}

int Game::Play()
{
	p1.FindEnemy(p2);
	p1.Actions();

	//

	p2.FindEnemy(p1);
	p2.Actions();

	//

	p1.CheckDead();
	p2.CheckDead();

	//

	return 0;
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

int Game::GetPause()
{
	return pause;
}
