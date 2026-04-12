#include "ISBN.h"
#include <iostream>

ISBN::ISBN() : ID()
{ }

ISBN::ISBN(unsigned* code) : ID()
{
	for (size_t i = 0; i < ISBNLen; i++)
	{
		if ((i == 0 && code[i] != 9) ||
			(i == 1 && code[i] != 7) ||
			(i == 2 && code[i] != 8))
		{
			throw std::invalid_argument("Invalid argument!\n");
		}
		ID[i] = code[i];
	}
}

void ISBN::print() const
{
	std::cout << "ISBN: ";
	for (size_t i = 0; i < ISBNLen; i++)
	{
		if(i == 3 || i == 4 || i == 6 || i == 12)
		{
			std::cout << "-";
		}
		std::cout << ID[i];
	}
}

const unsigned* ISBN::getISBN() const
{
	return ID;
}

bool ISBN::compareISBN(const ISBN& other) const
{
	for (size_t i = 3; i < ISBNLen; i++)
	{
		if (ID[i] != other.ID[i])
		{
			return false;
		}
	}
	return true;
}
