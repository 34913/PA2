#ifndef UniqID_H
#define UniqID_H

#include <set>
#include <cstdint>

template <typename T>
class UniqID
{
	std::set<T> available;
	
public:

	/**
	 * constructor.
	 *
	 * \param lowest - sets
	 */
	UniqID(T lowest = 0);

	/**
	 * clears set.
	 * 
	 */
	~UniqID();

	// methods

	/**
	 * gets uniq identifier, lowest as it can be.
	 *
	 * \return id as requested
	 */
	T getLowest();

	/**
	 * retrieves given id for reuse.
	 *
	 * \param id used id
	 */
	void Removed(T id);

};

extern UniqID<uint32_t> readyID;

template<typename T>
inline UniqID<T>::UniqID(T lowest)
{
	available.insert(lowest);
}

template<typename T>
inline UniqID<T>::~UniqID()
{
	available.clear();
}

template<typename T>
inline T UniqID<T>::getLowest()
{
	T temp = *available.begin();

	if (available.size() != 1) {
		available.erase(available.begin());
	}
	else {
		available.erase(available.begin());
		available.insert(temp + 1);
	}
	return temp;
}

template<typename T>
inline void UniqID<T>::Removed(T id)
{
	available.insert(id);
}

#endif
