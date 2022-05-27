#pragma once

#include <ostream>

class TypeCode
{
public:
	const int code;
	
	TypeCode(int code)
		:code(code)
	{}

	bool operator == (const TypeCode& t) const;

	bool operator != (const TypeCode& t) const;

	friend std::ostream& operator << (std::ostream& os, const TypeCode& x);

};
