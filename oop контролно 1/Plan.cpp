#include "Plan.h"
#include <iostream>

unsigned Plan::MAX_SUBJECTS = 40;

Plan::Plan(const unsigned maxCredits)
	:subjects(nullptr), maxCredits(maxCredits), currentCredits(0), subjectCount(0)
{
	subjects = new Subject*[MAX_SUBJECTS];
}

Plan::~Plan() noexcept
{
	for (unsigned i = 0; i < subjectCount; ++i)
	{
		delete subjects[i];
	}
	delete[] subjects;
}

bool Plan::operator+=(const Subject& other)
{
	if (subjectCount + 1 > MAX_SUBJECTS || 
		currentCredits + other.getCredits() > maxCredits)
	{
		return false;
	}

	for (unsigned i = 0; i < subjectCount; ++i)
	{
		if (*subjects[i] == other)
		{
			std::cout << "Subject already exists\n";
			return false;
		}
	}

	subjects[subjectCount] = new Subject(other);
	subjectCount++;
	currentCredits += other.getCredits();

	return true;
}

void Plan::operator-=(const Subject& other)
{
	for (unsigned i = 0; i < subjectCount; ++i)
	{
		if (subjects[i] != nullptr && *subjects[i] == other && 
			!subjects[i]->getHadExam())
		{
			currentCredits -= subjects[i]->getCredits();
            
			delete subjects[i];
		
			for (unsigned j = i; j < subjectCount - 1; ++j)
			{
				subjects[j] = subjects[j + 1];
			}

			subjects[subjectCount - 1] = nullptr;
			subjectCount--;
			
			i > 0 ? i-- : i = 0;
		}
	}
}

Subject& Plan::operator[](const unsigned idx) const
{
	return *subjects[idx];
}

Subject& Plan::operator[] (const unsigned idx)
{
	return *subjects[idx];
}

unsigned Plan::getMaxCredits() const
{
	return maxCredits;
}

unsigned Plan::getCurrentCredits() const
{
	return currentCredits;
}

unsigned Plan::getSubjectCount() const
{
	return subjectCount;
}

double Plan::getAverage() const
{
	double sum = 0;
	double exams = 0;

	for (unsigned i = 0; i < subjectCount; i++)
	{
		if(subjects[i]->getHadExam())
		{
			sum += subjects[i]->getGradeNum();
			exams++;
		}
	}

	return sum / exams;
}

bool Plan::gradeExam(const char* subjName, const Grade grade)
{
	if (subjName == nullptr || *subjName == '\0' || grade == Grade::Empty)
	{
		return false;
	}

	for(unsigned i = 0; i < subjectCount; ++i)
	{
		if (strcmp(subjName, subjects[i]->getName()) == 0 && (!subjects[i]->getHadExam() ||
			subjects[i]->getGrade() == Grade::Fail))
		{
			subjects[i]->setGrade(grade);
			subjects[i]->setHadExam(true);
			return true;
		}
	}
	return false;
}

void Plan::printSub()
{
	for (unsigned i = 0; i < subjectCount; ++i)
	{
		printSubject(*subjects[i]);
		std::cout << "\n";
	}
}