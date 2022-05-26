#include "Object.h"

Object::Object(const Point& coords, const Stats& values, const TypeCode& type)
	:coords(coords),
	values(values),
	type(type),
	id(readyID.getLowest())
{}

Object::Object(const Object& obj)
	:coords(obj.coords),
	values(obj.values),
	type(obj.type),
	id(readyID.getLowest())
{}

Object::~Object()
{
	readyID.Removed(id);
}

bool Object::operator==(const Object& obj) const
{
	return id == obj.id;
}

std::ostream& operator<<(std::ostream& os, const Object& x)
{
	x.Print(os);
	return os;
}

bool Object::HasInRange(Object& obj, double& len)
{
	len = coords.Length(obj.coords);

	return len <= values.range;
}

bool Object::Attack(Object& obj)
{
	return obj.Defend(*this);
}

bool Object::Defend(Object &obj)
{
	values.health -= obj.values.damage;
	return true;
}

bool Object::IsAlive() const
{
	return values.health > 0;
}

Point& Object::GetCoords()
{
	return coords;
}

void Object::AddStats(const Stats& add)
{
	values.Add(add);
}

void Object::Print(std::ostream& os) const
{
	os << id << " " << type << " " << coords << " " << values;
}

uint32_t Object::GetId() const
{
	return id;
}
