#pragma once

#ifndef BACKPACK_H
#define BACKPACK_H

#include "Book.h"

class Backpack
{
public:

	Backpack();
	Backpack(const Backpack& other);
	Backpack& operator=(const Backpack& other);
	~Backpack();

	const unsigned getBookCount() const;

	void addBook(Book& newBook);
	bool removeBook(ISBN& isbn);
	void transferToBinary(const char* fileName);
	Backpack restore(const char* fileName);
	
	void print() const;

private:
	Book* books;
	static const double maxWeight;
	double currentWeight;
	unsigned bookCount;
};

#endif