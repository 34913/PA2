#pragma once

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
	size_t GetHeight() const;

	/**
	 * Width getter.
	 * 
	 * \return width of playing field
	 */
	size_t GetWidth() const;

	/**
	 * Whole map getter.
	 * 
	 * \return map with its contents
	 */
	const std::vector<std::vector<char>>& GetArr() const;

};
