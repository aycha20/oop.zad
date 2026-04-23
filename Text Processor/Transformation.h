#pragma once

enum class TransformationType
{
	TO_UPPER,
	TO_LOWER,
	REPLACE_CHAR,
	REPLACE_STRING
};

class Transformation
{
public:
	Transformation();
	Transformation(const Transformation& other);
	Transformation& operator=(const Transformation& other);
	~Transformation();


	TransformationType type;
	bool active;

	char oldChar;
	char newChar;

	char* oldString;
	char* newString;
};