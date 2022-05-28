#pragma once

#include "MoneyNeeded.h"

#include <set>

class Money : public MoneyNeeded
{
	uint32_t golds;

	void Add() override;

public:

	Money(uint32_t starting = 0);

	void AddMoney(Object& obj);

	uint32_t GetMoney();

};
