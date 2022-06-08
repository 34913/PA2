// AntWars.cpp : Tento soubor obsahuje funkci main. Provádění programu se tam zahajuje a ukončuje.
//

#include <iostream>

#ifdef linux
// #include <SDL2/SDL_platform.h>
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_pixels.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#else
#include <SDL.h>
#include <SDL_pixels.h>
#endif

#include <chrono>

#include "UniqID.h"
#include "TypeCode.h"
#include "Stats.h"

#include "Object.h"
#include "Ant.h"
#include "MeleeAnt.h"
#include "RangedAnt.h"
#include "TankAnt.h"
#include "Base.h"

#include "Command.h"
#include "Player.h"
#include "AutoPlayer.h"
#include "Map.h"
#include "Point.h"

#include "MoneyNeeded.h"
#include "Money.h"
#include "TrainingTime.h"
#include "Level.h"

#include "Game.h"

/*! \mainpage AntWars Documentation
 *
 * \section intro_sec Introduction
 *
 * This is my documentation for my semester thesis AntWars in subject BI-PA2
 * \n
 * \author František Holý
 * \date 8.6.2022
 * \n
 * \n
 *
 * \subsection what_sec What is it about
 * This thesis is just about making possibly funny game to play about ants where player play against machine.
 * \n
 * I focused on making mainly 3 types of ants and base, took a little inspiration from one *ancient* online game named Age of War.
 * There is this similarity with having one base on each left and right side of playing field and player just trained 3 types of troops to go against their enemy.
 *
 * \subsection controls_sec Controling the game
 * Controlling the game is as simple as it can be
 * \n
 * \arg \c Loading or saving the games is in console, training troops on numpad (careful, you have to have focus on main app window) 1 to 3, which are melee, range and tank in this order.
 * \arg \c Switching which base trains now is with arrows, right and left.
 * \arg \c Arrows up and down are for controlling the speed of the game, up for faster game.
 * \arg \c Last is the space, which pauses or resumes the game, depends in which state it was.
 *
 * \n
 */

/**
 * Starts with init, sets window and renderer.
 * 
 * \param MainWindow SDL_Window ptr ref to be set
 * \param renderer SDL_Renderer ptr ref
 * \param show Map ref to get size of playing field
 * \return 0 if succeeded
 */
int begin(SDL_Window*& MainWindow, SDL_Renderer*& renderer, Map& show)
{
	SDL_Init(SDL_INIT_VIDEO);

	// creating the main window with game
	MainWindow = SDL_CreateWindow(
		"AntWars",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		show.GetWidth() * 10 + 300,
		show.GetHeight() * 10,
		0
	);

	if (MainWindow == NULL) {
		// error creating main window
		std::cout << "Error creating main window" << std::endl;
		return -1;
	}

	renderer = SDL_CreateRenderer(MainWindow, -1, 0);
	if (renderer == NULL) {
		// error creating renderer
		std::cout << "Error creating renderer" << std::endl;
		return -1;
	}

	return 0;
}

/**
 * Drawing circle function.
 * 
 * \param radius what might it be in a circle
 * \param x center coordinate
 * \param y center coordinate
 * \param renderer SDL_Renderer
 */
void DrawCircle(int radius, int x, int y, SDL_Renderer* renderer)
{
	// drawing the circle from left side to right side
	//	-> lowest x coord to highest for given radius
	// calculate the height and draw the line from highest point (y coord) to lowest of given radius
	// not the most accurate one, but cerstainly the fastest in this given 
	//	problem without any holes within the circleor compromises

	for (int low = 0; low <= radius; low++) {

		// calculate the distances using trigonometric functions
		int calc = (int)(sin(acos((double)low / radius)) * radius);

		// draw the line vertically
		//	-> x stays the same for starting and ending points
		SDL_RenderDrawLine(renderer,
			0 - low + x, y - calc,
			0 - low + x, y + calc);
		SDL_RenderDrawLine(renderer,
			low + x, y - calc,
			low + x, y + calc);
	}
}

/**
 * Printing out all the stuff of player.
 * 
 * \param p player
 * \param renderer SDL_Renderer
 * \param rect SDL_Rect
 */
void PrintOut(Player& p, SDL_Renderer* renderer, SDL_Rect* rect)
{
	for (auto& x : p.GetStuff()) {
		Point& temp = x.second.ptr->GetCoords();

		// decide which it is
		//	-> for base make the shape rectangle
		if (x.second.ptr->type == Base::type) {
			rect->x = temp.x * 10;
			rect->y = temp.y * 10;
			SDL_RenderFillRect(renderer, rect);
		}
		// for else, make it circle using my function
		else
			DrawCircle(5, temp.x * 10 + 5, temp.y * 10 + 5, renderer);
	}
}

/**
 * Handling the events.
 * 
 * \param g Game reference
 * \param event event ptr
 * \return returns 0 on success
 */
int Handle(Game& g, SDL_Event* event)
{
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT) {
			std::cout << "quit!" << std::endl;
			return 1;
		}

		// keys
		if (event->type == SDL_KEYDOWN) {
			if (event->key.repeat)
				break;

			try {
				switch (event->key.keysym.sym) {
				case SDLK_RIGHT:
					g.p1.Input(Command::nextBase);
					break;
				case SDLK_LEFT:
					g.p1.Input(Command::backBase);
					break;
				case SDLK_UP:
					g.Faster();
					break;
				case SDLK_DOWN:
					g.Slower();
					break;
				case SDLK_SPACE:
					g.RunStop();
					break;
				case SDLK_1:
					g.p1.Input(Command::trainMelee);
					break;
				case SDLK_2:
					g.p1.Input(Command::trainRange);
					break;
				case SDLK_3:
					g.p1.Input(Command::trainTank);
					break;
				default:
					break;
				}
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
			}
		}
		else if (event->type == SDL_WINDOWEVENT) {
			switch (event->window.event) {
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_FOCUS_LOST:
				g.RunStop(g.stop);
				break;
			}
		}
	}

	return 0;
}

// main

int main(int argc, char** args)
{
	// Handle the game itself
	Game g;

	// set the random seed
	// some random crap, determined by smashing fingers on num pad
	srand((unsigned long)time(NULL) + (rand() % 37));

	//Game game;
	if (g.Load() != 0) {
		return 0;
	}

	SDL_Init(SDL_INIT_VIDEO);

	// creating the main window with game
	SDL_Window* mainWindow = nullptr;
	SDL_Renderer* renderer = nullptr;

	try {
		if (begin(mainWindow, renderer, g.show) != 0)
			return -1;
	}
	catch(const std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		return -2;
	}

	// FPS
	// now on 75 (Frames Per Second)
	//	-> my monitor has 75 hertz
	int hertz = 75;
	double interval = (double)1000 / hertz;

	auto start = std::chrono::steady_clock::now();

	std::chrono::steady_clock::time_point last;
	std::chrono::steady_clock::time_point now;
	std::chrono::milliseconds millis;

	SDL_Event* event = new SDL_Event();

	//
	// main cycle
	//

	SDL_Rect* antRect = new SDL_Rect();
	antRect->w = antRect->h = 10;

	//SDL_Surface* Solid = TTF_RenderText_Solid(MyFont, "Solid", *RedColor);

	while(true) {
		if (g.p1.CheckBases() || g.p2.CheckBases())
			break;

		// set the time point
		now = std::chrono::steady_clock::now();

		// some action over specified amount of time
		// do something, like move all the ants
		millis = std::chrono::duration_cast<std::chrono::milliseconds>(now - last);
		if (g.GetRunning() && millis.count() >= g.GetPause()) {
			g.Play();
			last = now;
		}

		// saving resources by skipping the cycle
		//	-> 75 FPS, refresh just 75x times a second, save as much possible
		millis = std::chrono::duration_cast<std::chrono::milliseconds>(now - start);
		if (millis.count() < interval)
			continue;

		// printing FPS
		//std::cout << (double)1000 / millis.count() << std::endl;

		start = now;

		// event handling
		// like pressed buttons
		if (Handle(g, event) == 1)
			break;

		// background
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		// player1 objects
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		PrintOut(g.p1, renderer, antRect);
		
		// player2 objects
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		PrintOut(g.p2, renderer, antRect);

		// borders
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(renderer, antRect);
		for (int y = 0; y < g.show.GetHeight(); y++) {
			for (int x = 0; x < g.show.GetWidth(); x++) {

				if (g.show[y][x] == g.show.EMPTY)
					continue;

				antRect->x = x * 10;
				antRect->y = y * 10;
				SDL_RenderFillRect(renderer, antRect);

			}
		}

		// final printout
		//	-> JUST ONCE !!!
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	delete antRect;

	std::cout << "konec" << std::endl << g.p1.CheckBases() << " " << g.p2.CheckBases() << std::endl;

	return 0;
}
