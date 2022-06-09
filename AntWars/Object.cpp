 #include "Object.h"

Object::Object(const Point& coords, const Stats& values, const TypeCode& type)
	:coords(coords),
	values(values),
	type(type)
{}

Object::Object(const Object& obj)
	:coords(Point(obj.coords)),
	values(Stats(obj.values)),
	type(obj.type)
{}

Object::~Object()
{
	readyID.Removed(id);
}

Object& Object::operator=(const Object& obj)
{
	if (this == &obj)
		return *this;

	coords = Point(obj.coords);
	values = Stats(obj.values);
	
	return *this;
}

bool Object::operator==(const Object& obj) const
{
	return id == obj.id;
}

void Object::Set(const Point& coords, double health)
{
	this->coords = Point(coords);
	values.health = health;
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
	auto now = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count() < interval)
		return false;
	last = now;
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

Point Object::GetCoords(int multiplier)
{
	return Point(coords.x * multiplier, coords.y * multiplier);
}

void Object::AddStats(const Stats& add)
{
	values.Add(add);
}

void Object::Print(std::ostream& os) const
{
	os << type << " " << coords << " " << values.health;
}

uint32_t Object::GetId() const
{
	return id;
}
