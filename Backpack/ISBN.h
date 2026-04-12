#pragma once

#ifndef ISBN_H
#define ISBN_H

#include <stdexcept>

static constexpr size_t ISBNLen = 13;

class ISBN
{
public:
	ISBN();

	explicit ISBN(unsigned* code);

	void print() const;

	const unsigned* getISBN() const;
	 
	bool compareISBN(const ISBN& other) const;

private:
	unsigned ID[ISBNLen];
};

#endif