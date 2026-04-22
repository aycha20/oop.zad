#include "AlgebrVector.h"
#include <stdexcept>
#include <iostream>

AlgebrVector::AlgebrVector(const int newN)
{
	if (newN < 0)
	{
		throw std::out_of_range("Invalid size passed\n");
	}

	N = newN;

	try
	{
		data = new double[N] {};
	}
	catch (const std::bad_alloc& e)
	{
		std::cout << "Unexpected error" << e.what();
		throw;
	}
}

AlgebrVector::AlgebrVector()
	:N(0), data(nullptr)
{}

AlgebrVector::AlgebrVector(const AlgebrVector& other)
	:N(other.N), data(nullptr)
{
	if(other.N > 0)
	{
		data = new double[N];
		for (int i = 0; i < N; ++i)
		{
			data[i] = other.data[i];
		}
	}
}

AlgebrVector& AlgebrVector::operator=(const AlgebrVector& other)
{
	if (this != &other)
	{
		double* temp = nullptr;
		
		if(other.N > 0)
		{
			temp = new double[other.N];
			for (int i = 0; i < other.N; ++i)
			{
				temp[i] = other.data[i];
			}
		}

		delete[] data;

		data = temp;
		N = other.N;
	}
	return *this;
}

AlgebrVector::~AlgebrVector()
{
	delete[] data;
}

int AlgebrVector::getN() const
{
	return N;
}

void AlgebrVector::fillWithZero(AlgebrVector& a, int newN)
{
	if (newN < a.N)
	{
		throw std::out_of_range("Size too small\n");
	}

	AlgebrVector tempV(newN);

	for (int i = 0; i < a.N; ++i)
	{
		tempV.data[i] = a.data[i];
	}

	for (int i = a.N; i < newN; ++i)
	{
		tempV.data[i] = 0;
	}

	a = tempV;
}

double& AlgebrVector::operator[](const int idx)
{
	if (idx < 0 || idx >= N)
	{
		throw std::out_of_range("Invalid argument\n");
	}

	return data[idx];
}

double AlgebrVector::operator[](const int idx) const
{
	if (idx < 0 || idx >= N)
	{
		return 0;
	}

	return data[idx];
}

AlgebrVector& AlgebrVector::operator+=(const AlgebrVector& other)
{
	int size = N;

	if (N != other.N)
	{
		if (N < other.N)
		{
			fillWithZero(*this, other.N);
			size = other.N;
		}
		else
		{
			AlgebrVector tempOther(other);
			fillWithZero(tempOther, N);

			for (int i = 0; i < N; ++i)
			{
				data[i] += tempOther.data[i];
			}
			return *this;
		}
	}

	for (int i = 0; i < size; ++i)
	{
		data[i] += other.data[i];
	}

	return *this;
}

AlgebrVector& AlgebrVector::operator-=(const AlgebrVector& other)
{
	int size = N;

	if (N != other.N)
	{
		if (N < other.N)
		{
			fillWithZero(*this, other.N);
			size = other.N;
		}
		else
		{
			AlgebrVector tempOther(other);
			fillWithZero(tempOther, N);

			for (int i = 0; i < N; ++i)
			{
				data[i] -= tempOther.data[i];
			}
			return *this;
		}
	}

	for (int i = 0; i < size; ++i)
	{
		data[i] -= other.data[i];
	}

	return *this;
}

AlgebrVector& AlgebrVector::operator*=(const double n)
{
	for (int i = 0; i < N; ++i)
	{
		data[i] *= n;
	}

	return *this;
}

AlgebrVector& AlgebrVector::operator/=(const double n)
{
	if (n == 0)
	{
		throw std::out_of_range("Division by zero\n");
	}

	for (int i = 0; i < N; ++i)
	{
		data[i] /= n;
	}

	return *this;
}

AlgebrVector operator+(const AlgebrVector& a, const AlgebrVector& b)
{
	AlgebrVector res = a;
	
	res += b;
	
	return res;
}

AlgebrVector operator-(const AlgebrVector& a, const AlgebrVector& b)
{
	AlgebrVector res = a;

	res -= b;

	return res;
}

AlgebrVector operator*(const AlgebrVector& a, const int b)
{
	AlgebrVector res = a;

	res *= b;

	return res;
}

AlgebrVector operator/(const AlgebrVector& a, const int b)
{
	if (b == 0)
	{
		throw std::out_of_range("Invalid\n");
	}

	AlgebrVector res = a;

	res /= b;

	return res;
}

double AlgebrVector::operator%(const AlgebrVector& other) const
{
	int size = (N > other.N ? N : other.N);
	AlgebrVector temp1(*this);
	AlgebrVector tempOther(other);
	
	if (N != other.N)
	{
		if (N < other.N)
		{
			fillWithZero(temp1, other.N);
		}
		else
		{
			fillWithZero(tempOther, N);
		}
	}

	double res= 0;

	for (int i = 0; i < size; ++i)
	{
		res += temp1[i] * tempOther[i];
	}

	return res;
}
