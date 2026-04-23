#pragma warning(disable : 4996)

#include "TextProcessor.h"
#include <iostream>

TextProcessor::Transformation::Transformation()
	:type(), active(true), oldChar('\0'), newChar('\0'), oldString(nullptr), 
	newString(nullptr){}

TextProcessor::Transformation::Transformation(const Transformation& other)
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

TextProcessor::Transformation& TextProcessor::Transformation::operator=(const Transformation& other)
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

TextProcessor::Transformation::~Transformation()
{
	delete[] oldString;
	delete[] newString;
}

TextProcessor::TextProcessor(const char* newText)
	:TextProcessor()
{
	setText(newText);
}

TextProcessor::TextProcessor()
	:text(nullptr), transformations(nullptr), size(0), capacity(10) 
{
	text = new char[1];
	text[0] = '\0';
}

TextProcessor::TextProcessor(const TextProcessor& other)
{
	text = new char[strlen(other.text) + 1];
	strcpy(text, other.text);

	try
	{
		transformations = new Transformation[other.size];
		for (int i = 0; i < other.size; ++i)
		{
			transformations[i] = other.transformations[i];
		}
	}
	catch (const std::bad_alloc& e)
	{
		delete[] text;
		text = nullptr;
		throw;
	}

	size = other.size;
	capacity = other.capacity;
}

TextProcessor& TextProcessor::operator=(const TextProcessor& other)
{
	if (this != &other)
	{
		char* temp = new char[strlen(other.text) + 1];
		strcpy(temp, other.text);

		Transformation* temp1 = nullptr;
		try
		{
			temp1 = new Transformation[other.size];
			for (int i = 0; i < other.size; ++i)
			{
				temp1[i] = other.transformations[i];
			}
		}
		catch (const std::bad_alloc& e)
		{
			delete[] temp;
			temp = nullptr;
			throw;
		}

		delete[] text;
		text = temp;

		delete[] transformations;
		transformations = temp1;

		size = other.size;
		capacity = other.capacity;
	}
	return *this;
}

TextProcessor::~TextProcessor()
{
	delete[] text;
	delete[] transformations;
}

const char* TextProcessor::getText() const
{
	return text;
}

const  TextProcessor::Transformation* TextProcessor::getTransformations()
{
	return transformations;
}

int TextProcessor::getSize() const
{
	return size;
}

int TextProcessor::getCapacity() const
{
	return capacity;
}

void TextProcessor::setText(const char* newText)
{
	if (newText == nullptr || newText[0] == '\0')
	{
		throw std::invalid_argument("No text passed\n");
	}

	char* temp = new char[strlen(newText) + 1];
	strcpy(temp, newText);

	delete[] text;

	text = temp;
}

void TextProcessor::setTransformations(const Transformation* newTransformations, const int newSize)
{
	if (newTransformations == nullptr)
	{
		throw std::invalid_argument("No text passed\n");
	}
	if (newSize > capacity)
	{
		throw std::out_of_range("Size too big\n");
	}

	Transformation* temp = new Transformation[newSize];

	for (int i = 0; i < newSize; ++i)
	{
		temp[i] = newTransformations[i];
	}

	delete[] transformations;

	transformations = temp;
}

void TextProcessor::setCapacity(const int newCapacity)
{
	if (newCapacity < size)
	{
		throw std::out_of_range("New capacity too small\n");
	}

	capacity = newCapacity;
}

TextProcessor& TextProcessor::operator+=(const char* newText)
{
	if (newText == nullptr)
	{
		throw std::invalid_argument("Empty argument passed\n");
	}

	int newLen = strlen(text) + strlen(newText) + 1;

	char* temp = new char[newLen];
	strcpy(temp, text);

	int idx = 0;

	for (int i = strlen(text); i < newLen; ++i)
	{
		temp[i] = newText[idx];
		++idx;
	}

	delete[] text;

	text = temp;

	return *this;
}

TextProcessor& TextProcessor::operator+=(const Transformation& newTransformation)
{
	int newSize = size + 1;
	if (newSize > capacity)
	{
		throw std::out_of_range("Capacity full!\n");
	}

	Transformation* temp = new Transformation[newSize];

	for (int i = 0; i < size; ++i)
	{
		temp[i] = transformations[i];
	}

	temp[newSize - 1] = newTransformation;

	delete[] transformations;
	transformations = temp;
	size += 1;

	return *this;
}

TextProcessor& TextProcessor::operator-=(int idx)
{
	if (idx >= size || size == 0)
	{
		throw std::out_of_range("Index is invalid\n");
	}

	Transformation* temp = new Transformation[size - 1];

	int index = 0;

	for (int i = 0; i < size; ++i)
	{
		if (i == idx)
		{
			continue;
		}
		try
		{
			temp[index++] = transformations[i]; // moje da hvurli neshto
		}
		catch (...)
		{
			delete[] temp;
			temp = nullptr;
			throw;
		}
	}

	size -= 1;
	delete[] transformations;
	transformations = temp;
	return *this;
}

TextProcessor& TextProcessor::operator-()
{
	for (int i = 0; i < size; ++i)
	{
		transformations[i].active == false ? (transformations[i].active = true) :
			(transformations[i].active = false);
	}
	return *this;
}

char& TextProcessor::operator[](const int idx)
{
	if (idx < 0 || idx >= strlen(text))
	{
		throw std::out_of_range("Index is out of range\n");
	}
	return text[idx];
}

char TextProcessor::operator[](const int idx) const
{
	if (idx < 0 || idx >= strlen(text))
	{
		throw std::out_of_range("Index is out of range\n");
	}
	return text[idx];
}

char* TextProcessor::getProcessedText() const
{
	char* result = new char[strlen(text) + 1];
	strcpy(result, text);
	int resLen = strlen(result);

	for (int i = 0; i < size; ++i)
	{
		if (!transformations[i].active)
		{
			continue;
		}

		if (transformations[i].type == TextProcessor::TransformationType::TO_UPPER)
		{
			for (int j = 0; j < resLen; ++j)
			{
				if (result[j] >= 'a' && result[j] <= 'z')
				{
					result[j] -= ('a' - 'A');
				}
			}
		}

		else if (transformations[i].type == TextProcessor::TransformationType::TO_LOWER)
		{
			for (int j = 0; j < resLen; ++j)
			{
				if (result[j] >= 'A' && result[j] <= 'Z')
				{
					result[j] += ('a' - 'A');
				}
			}
		}

		else if (transformations[i].type == TextProcessor::TransformationType::REPLACE_CHAR)
		{
			for (int j = 0; j < resLen; ++j)
			{
				if (result[j] == transformations[i].oldChar)
				{
					result[j] = transformations[i].newChar;
				}
			}
		}

		else if (transformations[i].type == TextProcessor::TransformationType::REPLACE_STRING)
		{
			char* newRes = nullptr;
			try
			{
				newRes = replaceString(result, transformations[i].oldString,
					transformations[i].newString);
			}
			catch (const std::bad_alloc& e)
			{
				delete[] result;
				result = nullptr;
				throw;
			}
			catch (...)
			{
				delete[] result;
				result = nullptr;
				throw;
			}

			delete[] result;
			result = newRes;
			resLen = strlen(result);

		}
	}
	return result;
}

std::ostream& operator<<(std::ostream& os, const TextProcessor& txt) 
{
	char* result = txt.getProcessedText();
	os << result;
	delete[] result;
	return os;
}

bool found(const char* text, const char* pattern, int index)
{
	if (!text || !pattern)
	{	
		return false;
	}

	int i = 0;
	while (pattern[i] != '\0')
	{
		if (text[index + i] == '\0' || text[index + i] != pattern[i])
		{
			return false;
		}
		++i;
	}
	return true;
}

int countOldString(const char* text, const char* oldStr)
{
	if (!text || !oldStr || oldStr[0] == '\0')
	{
		return 0;
	}

	int count = 0;
	int oldLen = strlen(oldStr);
	int textLen = strlen(text);

	for (int i = 0; i < textLen; ++i)
	{
		if (found(text, oldStr, i))
		{
			++count;
			i += oldLen - 1;
		}
	}

	return count;
}

char* replaceString(const char* text, const char* oldStr, const char* newStr)
{
	if (!text)
	{
		throw std::invalid_argument("Invalid text passed\n");
	}
	
	if(!oldStr || !newStr || oldStr[0] == '\0')
	{
		char* copy = new char[strlen(text) + 1];
		for (int i = 0; text[i] != '\0'; i++)
		{
			copy[i] = text[i];
		}

		copy[strlen(text)] = '\0';
		return copy;
	}

	int textLen = strlen(text);
	int oldLen = strlen(oldStr);
	int newLen = strlen(newStr);

	int count = countOldString(text, oldStr);

	int resLen = textLen + count * (newLen - oldLen);

	char* res = new char[resLen + 1];

	int i = 0, j = 0;

	while (text[i] != '\0')
	{
		if (found(text, oldStr, i))
		{
			for (int k = 0; k < newLen; k++)
			{
				res[j] = newStr[k];
				++j;
			}
			i += oldLen;
		}
		else
		{
			res[j] = text[i];
			++j;
			++i;
		}
	}
	res[j] = '\0';
	return res;
}

bool operator==(const TextProcessor& a, const TextProcessor& b)
{
	char* resA = a.getProcessedText();
	char* resB = b.getProcessedText();

	int aLen = strlen(resA);
	int bLen = strlen(resB);

	if (aLen != bLen)
	{
		delete[] resA;
		delete[] resB;
		return false;
	}

	for (int i = 0; i < aLen; ++i)
	{
		if (resA[i] != resB[i])
		{
			delete[] resA;
			delete[] resB;
			return false;
		}
	}

	delete[] resA;
	delete[] resB;

	return true;
}
