#include "TypeCode.h"

bool TypeCode::operator==(const TypeCode& t) const
{
    return this->code == t.code;
}
