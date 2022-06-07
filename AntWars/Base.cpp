#include "Base.h"

TypeCode Base::type(1);

Base::Base(const Point& coords)
    :Object(coords, Stats(5.0, 15.0, 150.0), Base::type)
{}
