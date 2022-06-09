#include "Map.h"

Map::Map()
{}

std::ostream& operator<<(std::ostream& os, Map& obj)
{
	os << obj.GetWidth() << " " << obj.GetHeight() << std::endl;
	for (int i = 0; i < obj.GetHeight(); i++) {
		for (int w = 0; w < obj.GetWidth(); w++) {

			if (obj.arr[i][w] != Map::WALL && obj.arr[i][w] != Map::EMPTY)
				os.put(Map::EMPTY);
			else
				os.put(obj.arr[i][w]);
		}
		os << std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, Map& obj)
{
	obj.arr.clear();

	int width, height;
	if (!(is >> width >> height))
		throw std::invalid_argument("Cant read size of map, corrupted");

	if (width <= 0 || height <= 0)
		throw std::invalid_argument("Map file corrupted, wrong size of map");

	char ch = is.get();
	obj.arr.clear();
	for (int i = 0; i < height; i++) {
		obj.arr.push_back({});
		for (int w = 0; w < width; w++) {

			if (!is.good())
				throw std::invalid_argument("Map file corrupted, not enough characters in map");
			ch = is.get();
			obj.arr[i].push_back(ch);
		}

		ch = is.get();
		while (ch != '\n') {
			if (!is.good())
				throw std::invalid_argument("Map file corrupted, wrong line ending");

			ch = is.get();
		}
	}

	return is;
}

std::vector<char>& Map::operator[](size_t x)
{
	if (x < 0 || x >= arr.size())
		throw std::invalid_argument("Out of bounds");
	return arr[x];
}

int Map::GetHeight() const
{
	return (int)arr.size();
}

int Map::GetWidth() const
{
	if (GetHeight() == 0)
		return 0;
	return (int)arr[0].size();
}

const std::vector<std::vector<char>>& Map::GetArr() const
{
	return arr;
}
