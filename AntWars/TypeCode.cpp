#include "TypeCode.h"

bool TypeCode::operator==(const TypeCode& t) const
{
    return this->code == t.code;
}

bool TypeCode::operator!=(const TypeCode& t) const
{
    return !(*this == t);
}

std::ostream& operator<<(std::ostream& os, const TypeCode& x)
{
    os << x.code;
    return os;
}
