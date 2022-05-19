#include "Object.h"

bool Object::InRange(const Object& obj) const
{
	float temp[2] = { 0,0 };
	temp[0] = coords.x - obj.coords.x;
	temp[1] = coords.y - obj.coords.y;

	double len = 0;
	for (int i = 0; i < 2; i++)
		len += pow(temp[i], 2);

	// count the length from one to another
	len = sqrt(len);

	return len <= values.range;
}

void Object::Attack(Object& obj)
{
	obj.Defend(values.damage);
}

void Object::Defend(const int dmg)
{
	values.health -= dmg;
}

bool Object::IsAlive() const
{
	return values.health > 0;
}

const Point& Object::GetCoords() const
{
	return coords;
}
