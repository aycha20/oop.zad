#pragma once

#ifndef ALGEBRVECTOR_H
#define ALGEBRVECTOR_H

class AlgebrVector
{
public:

	AlgebrVector(const int newN);

	AlgebrVector();
	AlgebrVector(const AlgebrVector& other);
	AlgebrVector& operator=(const AlgebrVector& other);
	~AlgebrVector();

	int getN() const;

	void fillWithZero(AlgebrVector& a, int newN);

	double& operator[](const int idx);
	const double operator[](const int idx) const;

	AlgebrVector& operator+=(const AlgebrVector& other);
	AlgebrVector& operator-=(const AlgebrVector& other);
	AlgebrVector& operator*=(const int n);
	AlgebrVector& operator/=(const int n);

	double operator%(const AlgebrVector& other);

private:
	int N = 0;
	double* data = nullptr;
};

AlgebrVector operator+(const AlgebrVector& a, const AlgebrVector& b);
AlgebrVector operator-(const AlgebrVector& a, const AlgebrVector& b);
AlgebrVector operator*(const AlgebrVector& a, const int b);
AlgebrVector operator/(const AlgebrVector& a, const int b);


#endif