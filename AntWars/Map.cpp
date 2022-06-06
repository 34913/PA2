#include "Map.h"

Map::Map()
{}

std::ostream& operator<<(std::ostream& os, Map& obj)
{
	os << obj.GetWidth() << " " << obj.GetHeight() << std::endl;
	for (int i = 0; i < obj.GetHeight(); i++) {
		for (int w = 0; w < obj.GetWidth(); w++) {

			os.put(obj.arr[i][w]);
		}
		os << std::endl;
	}

	return os;
}

std::istream& operator>>(std::istream& is, Map& obj)
{
	int width, height;
	if (!(is >> width >> height))
		throw std::invalid_argument("Cant read size of playing field, corrupted");

	if (width <= 0 || height <= 0)
		throw std::invalid_argument("File corrupted, wrong size of map");

	char ch = is.get();
	obj.arr.clear();
	for (int i = 0; i < height; i++) {
		obj.arr.push_back({});
		for (int w = 0; w < width; w++) {

			if (!is.good())
				throw std::invalid_argument("File corrupted, not enough characters in map");
			ch = is.get();
			obj.arr[i].push_back(ch);
		}
		if ((ch = is.get()) != '\n' || !is.good())
			throw std::invalid_argument("File corrupted, wrong line ending");
	}

	return is;
}

std::vector<char>& Map::operator[](size_t x)
{
	if (x < 0 || x >= arr.size())
		throw std::invalid_argument("Out of bounds");
	return arr[x];
}

size_t Map::GetHeight() const
{
	return arr.size();
}

size_t Map::GetWidth() const
{
	if (GetHeight() == 0)
		return 0;
	return arr[0].size();
}

const std::vector<std::vector<char>>& Map::GetArr() const
{
	return arr;
}
