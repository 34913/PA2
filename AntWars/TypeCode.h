#ifndef TypeCode_H
#define TypeCode_H

#include <fstream>

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

#endif
