#pragma once

#include <set>

template <typename T>
class UniqID
{
	std::set<T> available;

	T lowest;

public:

	/**
	 * constructor.
	 *
	 * \param lowest - sets
	 */
	UniqID(T lowest = 0);

	/**
	 * gets uniq identifier, lowest as it can be.
	 *
	 * \return id as requested
	 */
	T getLowest();

	/**
	 * retrieves given id for reuse.
	 *
	 * \param used id
	 */
	void Removed(T id);

};

static UniqID<uint32_t> readyID(0);
