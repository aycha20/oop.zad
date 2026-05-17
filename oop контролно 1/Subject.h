#pragma once

enum class Grade
{
	Fail,
	Average,
	Good,
	Very_Good,
	Excellent,
	Empty
};

class Subject
{
public:
	Subject(const char* name, const char* annotation, 
		const unsigned credits, const bool hadExam, const Grade grade);

	Subject();
	Subject(const Subject& other);
	Subject& operator=(const Subject& other);
	~Subject();

	const char* getName() const;
	const char* getAnnotation() const;
	unsigned getCredits() const;
	bool getHadExam() const;
	Grade getGrade() const;
	unsigned getGradeNum() const;

	void setName(const char* newName);
	void setAnnotation(const char* newAnnotation);
	void setHadExam(const bool didHaveExam);
	void setGrade(const Grade grade);

private:
	char* name;
	char* annotation;
	const unsigned credits;
	bool hadExam;
	Grade grade;
};

bool operator> (const Subject& lhs, const Subject& rhs);
bool operator< (const Subject& lhs, const Subject& rhs);
bool operator==(const Subject& lhs, const Subject& rhs);

void printSubject(const Subject& subj);