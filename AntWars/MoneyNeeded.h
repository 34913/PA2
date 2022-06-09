#ifndef MoneyNeeded_H
#define MoneyNeeded_H

#include "Base.h"
#include "RangedAnt.h"
#include "MeleeAnt.h"
#include "TankAnt.h"

#include "Object.h"

#include <map>

#include <cstdint>
#include <fstream>

class MoneyNeeded
{
protected:
	
	// amounts to specified int type
	std::map<int, uint16_t> amount;

	/**
	* Prints out in the format it is able to read it afterwards.
	*
	* \param os ostream
	*/
	virtual void Print(std::ostream& os);

	/**
	 * Reading from input.
	 *
	 * \param is istream
	 */
	virtual void Scan(std::istream& is);

public:

	// operators

	uint16_t operator [](Object& obj);

	uint16_t operator[](TypeCode& type);

	uint16_t operator[](int code);

	friend std::ostream& operator << (std::ostream& os, MoneyNeeded& obj);

	friend std::istream& operator >> (std::istream& is, MoneyNeeded& obj);

	// methods

	/**
	 * Increase the amount for each item.
	 * 
	 */
	virtual void Up();

	/**
	 * Adds on start basic objects and their price/cost (depends on class using)
	 *
	 */
	virtual void begin();

};

#endif
