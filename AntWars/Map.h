#ifndef Map_H
#define Map_H

#include <vector>
#include <fstream>

class Map
{
	// playing field
	std::vector<std::vector<char>> arr;

public:

	// accesible constants

	static const char EMPTY = ' ';
	static const char WALL = '#';

	static const char ANT_P1 = 'X';
	static const char ANT_P2 = 'O';

	static const char BASE_P1 = 'P';
	static const char BASE_P2 = 'R';


	// cons/destructor

	Map();

	// operators

	friend std::ostream& operator << (std::ostream& os, Map& obj);

	friend std::istream& operator >> (std::istream& is, Map& obj);

	std::vector<char>& operator [] (size_t x);

	// methods



	// getters

	/**
	 * Height getter.
	 * 
	 * \return height of playing field
	 */
	int GetHeight() const;

	/**
	 * Width getter.
	 * 
	 * \return width of playing field
	 */
	int GetWidth() const;

	/**
	 * Whole map getter.
	 * 
	 * \return map with its contents
	 */
	const std::vector<std::vector<char>>& GetArr() const;

};

#endif
