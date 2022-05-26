#include "TypeCode.h"

bool TypeCode::operator==(const TypeCode& t) const
{
    return this->code == t.code;
}

std::ostream& operator<<(std::ostream& os, const TypeCode& x)
{
    os << x.code;
    return os;
}
