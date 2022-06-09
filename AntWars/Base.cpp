#include "Base.h"

TypeCode Base::type(1);

Base::Base(const Point& coords)
    :Object(coords, Stats(5.0, 8.0, 150.0), Base::type)
{}

bool Base::Attack(Object & obj)
{
    if (++counter == 5)
        values.range += add;
    bool r = Object::Attack(obj);

    if (counter == 5) {
        values.range -= add;
        counter = 0;
    }
    return r;
}
