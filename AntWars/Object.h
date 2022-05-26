#pragma once

#include "Point.h"
#include "Stats.h"
#include "TypeCode.h"
#include "UniqID.h"

#include <math.h>
#include <ostream>

class Object
{
protected:

	Point coords;

	Stats values;

	const uint32_t id;

public:

	// accesible constants

	const TypeCode& type;

	// construct + destruct

	Object(const Point& coords, const Stats& values, const TypeCode& type);
	
	Object(const Object& obj);

	~Object();

	// operators

	Object& operator = (const Object& obj);

	bool operator == (const Object& obj) const;

	friend std::ostream& operator << (std::ostream& os, const Object& x);

	// methods

	bool HasInRange(Object& obj, double& len);

	/**
	 * method to attack some other object.
	 * 
	 * \param obj to attack
	 * \return true if hit
	 */
	virtual bool Attack(Object& obj);

	/**
	 * method to defend from another obj.
	 * 
	 * \param obj to defend from
	 * \return true if damage done
	 */
	virtual bool Defend(Object& obj);

	/**
	 * Check whether the object is still alive.
	 * 
	 * \return true if is alive
	 */
	virtual bool IsAlive() const;

	/**
	 * get coordinates of class Point.
	 * 
	 * \return coords reference
	 */
	Point& GetCoords();

	/**
	 * Adds statistics.
	 * 
	 * \param add Stats reference
	 */
	void AddStats(const Stats& add);

	void virtual Print(std::ostream& os) const;

	uint32_t GetId() const;

};
