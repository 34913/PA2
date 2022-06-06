#pragma once

#include "Point.h"
#include "Stats.h"
#include "TypeCode.h"
#include "UniqID.h"

#include <math.h>
#include <ostream>

class Object abstract
{
protected:

	// position
	Point coords;

	// stats of this unit
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

	/**
	 * Set the initial values upon creating.
	 * 
	 * \param coords 
	 * \param health
	 */
	void Set(const Point& coords, double health);

	/**
	 * Determines if given object is in range of this object.
	 * 
	 * \param obj object to determine whether or not is in this range
	 * \param len to save length between them
	 * \return true if given object is in range of this
	 */
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

	/**
	 * printing in stream.
	 * 
	 * \param os stream to print in
	 */
	void virtual Print(std::ostream& os) const;

	/**
	 * returns uniq id for this object.
	 * 
	 * \return id
	 */
	uint32_t GetId() const;

};
