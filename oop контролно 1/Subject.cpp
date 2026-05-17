#define _CRT_SECURE_NO_WARNINGS
#include "Subject.h"
#include <stdexcept>
#include <iostream>
#include <cstring>

Subject::Subject(const char* name, const char* annotation,
	const unsigned credits, const bool hadExam, const Grade grade)
	:credits(credits), hadExam(hadExam)
{
	if (name == nullptr || *name == '\0' || annotation == nullptr || *annotation == '\0')
	{
		throw std::invalid_argument("Name/annotation is null or empty\n");
	}

	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);

	try
	{
		this->annotation = new char[strlen(annotation) + 1];
		strcpy(this->annotation, annotation);
	}
	catch (...)
	{
		delete this->name;
		this->name = nullptr;
		throw;
	}

	if(hadExam)
	{
		setGrade(grade);
	}
	else
	{
		setGrade(Grade::Empty);
	}
}

Subject::Subject()
	:name(nullptr), annotation(nullptr), credits(0), hadExam(false),
      grade(Grade::Empty) { }

Subject::Subject(const Subject& other)
	:name(nullptr), annotation(nullptr), credits(other.credits), hadExam(other.hadExam),
	grade(Grade::Empty)
{
	if (other.name == nullptr || *other.name == '\0' || 
		other.annotation == nullptr || *other.annotation == '\0')
	{
		throw std::invalid_argument("Name/annotation is null or empty\n");
	}

	name = new char[strlen(other.name) + 1];
	strcpy(name, other.name);

	try
	{
		annotation = new char[strlen(other.annotation) + 1];
		strcpy(annotation, other.annotation);
	}
	catch (...)
	{
		delete name;
		name = nullptr;
		throw;
	}

	if (hadExam)
	{
		setGrade(grade);
	}
	else
	{
		setGrade(Grade::Empty);
	}
}

Subject& Subject::operator=(const Subject& other)
{
	if (this != &other)
	{
		setName(other.name);

		try
		{
			setAnnotation(other.annotation);
		}
		catch (...)//invalid_argument ili bad_alloc
		{
			delete[] name;
			name = nullptr;
			throw;
		}

		hadExam = other.hadExam;

		if (hadExam)
		{
			setGrade(grade);
		}
		else
		{
			setGrade(Grade::Empty);
		}
	}
	return *this;
}

Subject::~Subject() noexcept
{
	delete[] name;
	delete[] annotation;
}

const char* Subject::getName() const
{
	return name;
}

const char* Subject::getAnnotation() const
{
	return annotation;
}

unsigned Subject::getCredits() const
{
	return credits;
}

bool Subject::getHadExam() const
{
	return hadExam;
}

Grade Subject::getGrade() const
{
	return grade;
}

unsigned Subject::getGradeNum() const
{
	switch (grade)
	{
	case Grade::Fail:
		return 2;
		break;
	case Grade::Average:
		return 3;
		break;
	case Grade::Good:
		return 4;
		break;
	case Grade::Very_Good:
		return 5;
		break;
	case Grade::Excellent:
		return 6;
		break;
	}
}

void Subject::setName(const char* newName)
{
	if (newName == nullptr || *newName == '\0')
	{
		throw std::invalid_argument("Empty name \n");
	}

	char* tempName = new char[strlen(newName) + 1];

	strcpy(tempName, newName);

	delete[] name;

	name = tempName;
}

void Subject::setAnnotation(const char* newAnnotation)
{
	if (newAnnotation == nullptr || *newAnnotation == '\0')
	{
		throw std::invalid_argument("Empty name \n");
	}

	char* tempA = new char[strlen(newAnnotation) + 1];

	strcpy(tempA, newAnnotation);

	delete[] annotation;

	annotation = tempA;
}

void Subject::setHadExam(const bool didHaveExam)
{
	hadExam = didHaveExam;
}

void Subject::setGrade(const Grade grade)
{
	this->grade = grade;
}

bool operator> (const Subject& lhs, const Subject& rhs)
{
	return !(lhs < rhs);
}

bool operator< (const Subject& lhs, const Subject& rhs)
{
	if (lhs.getCredits() < rhs.getCredits())
	{
		return true;
	}
	else if(lhs.getCredits() == rhs.getCredits() && 
		strlen(lhs.getAnnotation()) < strlen(rhs.getAnnotation()))
	{
		return true;
	}
	return false;
}

bool operator==(const Subject& lhs, const Subject& rhs)
{
	if (strcmp(lhs.getName(), rhs.getName()) == 0 &&
		strcmp(lhs.getAnnotation(), rhs.getAnnotation()) == 0 &&
		lhs.getCredits() == rhs.getCredits())
	{
		return true;
	}
	return false;
}

void printSubject(const Subject& subj)
{
	std::cout << subj.getName() << "\n"
		<< subj.getAnnotation() << "\n"
		<< subj.getCredits() << "\n";

	subj.getHadExam() == true ? std::cout << "Exam was conducted \n" : 
		std::cout << "Exam has not been conducted\n";

	switch (subj.getGrade())
	{
	case Grade::Fail:
		std::cout << "Fail (2)" << "\n";
		break;
	case Grade::Average:
		std::cout << "Average (3)" << "\n";
		break;
	case Grade::Good:
		std::cout << "Good (4)" << "\n";
		break;
	case Grade::Very_Good:
		std::cout << "Very Good (5)" << "\n";
		break;
	case Grade::Excellent:
		std::cout << "Excellent (6)" << "\n";
		break;
	case Grade::Empty:
		std::cout << "No Grade" << "\n";
		break;
	}
}