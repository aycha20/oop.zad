#pragma once

#include "Subject.h"

class Plan
{
public:
	Plan(const unsigned maxCredits);
	
	Plan(const Plan& other) = delete;
	Plan& operator=(const Plan& other) = delete;
	~Plan();

	bool operator+=(const Subject& other);
	void operator-=(const Subject& other);

	Subject& operator[](const unsigned idx) const;
	Subject& operator[] (const unsigned idx);

	unsigned getMaxCredits() const;
	unsigned getCurrentCredits() const;
	unsigned getSubjectCount() const;

	double getAverage() const;

	bool gradeExam(const char* subjName, const Grade grade);
	void printSub();

private:
	Subject** subjects;
	const unsigned maxCredits;
	unsigned currentCredits;
	unsigned subjectCount;

	static unsigned MAX_SUBJECTS;
};

