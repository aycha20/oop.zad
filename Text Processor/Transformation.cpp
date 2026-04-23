
#include "Transformation.h"
#include <iostream>

Transformation::Transformation()
	:type(), active(true), oldChar('\0'), newChar('\0'), oldString(nullptr),
	newString(nullptr) {
}

Transformation::Transformation(const Transformation& other)
	:type(other.type), active(other.active), oldChar(other.oldChar), newChar(other.newChar),
	oldString(nullptr), newString(nullptr)
{
	try
	{
		if (other.oldString)
		{
			oldString = new char[strlen(other.oldString) + 1];
			strcpy(oldString, other.oldString);
		}
		if (other.newString)
		{
			newString = new char[strlen(other.newString) + 1];
			strcpy(newString, other.newString);
		}
	}
	catch (...)
	{
		delete[] oldString;
		oldString = nullptr;
		delete[] newString;
		newString = nullptr;
		throw;
	}
}

Transformation& Transformation::operator=(const Transformation& other)
{
	if (this != &other)
	{
		if (other.oldString == nullptr)
		{
			delete[] oldString;
			oldString = nullptr;
			return *this;
		}

		char* temp1 = new char[strlen(other.oldString) + 1];
		strcpy(temp1, other.oldString);

		char* temp2 = nullptr;

		try
		{
			temp2 = new char[strlen(other.newString) + 1];
			strcpy(temp2, other.newString);
		}
		catch (const std::bad_alloc& e)
		{
			std::cout << e.what();
			delete[] temp1;
			temp1 = nullptr;
			throw;
		}

		delete[] oldString;
		delete[] newString;

		oldString = temp1;
		newString = temp2;

		type = other.type;
		active = other.active;
		oldChar = other.oldChar;
		newChar = other.newChar;
	}
	return *this;
}

Transformation::~Transformation()
{
	delete[] oldString;
	delete[] newString;
}