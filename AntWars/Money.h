#ifndef Money_H
#define Money_H

#include "MoneyNeeded.h"

#include "MeleeAnt.h"
#include "RangedAnt.h"
#include "TankAnt.h"
#include "Base.h"

#include <set>

class Money : public MoneyNeeded
{
	uint32_t golds;

	void Print(std::ostream& os) override;

	void Scan(std::istream& is) override;

public:

	Money(uint32_t starting = 0);

	/**
	 * Adds money based on type of given obj.
	 * 
	 * \param obj to be received money from
	 */
	void AddMoney(Object& obj);

	/**
	 * Removes money base on type of given obj.
	 * 
	 * \param obj to be deducted money from
	 */
	void RemoveMoney(uint16_t cost);

	/**
	 * Amount of money.
	 * 
	 * \return golds
	 */
	uint32_t GetMoney();

	void begin() override;

};

#endif
