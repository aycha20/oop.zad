#pragma once

#include <iostream>

class TextProcessor
{
public:
	enum class TransformationType
	{
		TO_UPPER, 
		TO_LOWER, 
		REPLACE_CHAR, 
		REPLACE_STRING
	};

	struct Transformation
	{
		TransformationType type;
		bool active;

		char oldChar;
		char newChar;

		char* oldString;
		char* newString;

		Transformation();
		Transformation(const Transformation& other);
		Transformation& operator=(const Transformation& other);
		~Transformation();
	};

	TextProcessor(const char* newText);
	
	TextProcessor();
	TextProcessor(const TextProcessor& other);
	TextProcessor& operator=(const TextProcessor& other);
	~TextProcessor();

	const char* getText() const;
	const Transformation* getTransformations();
	int getSize() const;
	int getCapacity() const;
	
	void setText(const char* newText);
	void setTransformations(const Transformation* newTransformations, const int newSize);
	void setCapacity(const int newCapacity);

	TextProcessor& operator+=(const char* newText);
	TextProcessor& operator+=(const Transformation& newTransformation);
	TextProcessor& operator-=(int idx);

	TextProcessor& operator-();


	char& operator[](const int idx);
	char operator[](const int idx) const;

	friend std::ostream& operator<<(std::ostream& os, const TextProcessor& txt);
	friend bool operator==(const TextProcessor& a, const TextProcessor& b);

private:
	char* text;
	Transformation* transformations;
	int size;
	int capacity;
	char* getProcessedText() const;
};


