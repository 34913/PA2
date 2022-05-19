#pragma once

class TypeCode
{
public:
	int code;
	
	TypeCode(int code)
		:code(code)
	{}

	bool operator == (const TypeCode& t) const;
};
