#include "UniqID.h"

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
