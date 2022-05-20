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

extern UniqID<uint32_t> readyID;

template<typename T>
inline UniqID<T>::UniqID(T lowest)
	:lowest(lowest)
{
	available.insert(lowest);
}

template<typename T>
inline T UniqID<T>::getLowest()
{
	T temp = lowest;
	if (available.size() > 1) {
		available.erase(lowest);
		lowest = *available.begin();
	}
	else
		available.insert(lowest++);
	return temp;
}

template<typename T>
inline void UniqID<T>::Removed(T id)
{
	if (id < lowest)
		lowest = id;
	available.insert(id);
}
