#pragma once

#ifndef BOOK_H
#define BOOK_H

#include "ISBN.h"

static const size_t MAX_TEXT_LEN = 100;

class Book
{
public:
	Book(char* newTitle, char* newAuthor, float newWeight, ISBN newISBN);

	Book();
	Book(const Book& other);
	Book& operator=(const Book& other);
	~Book();

	void setTitle(const char* newTitle);
	void setAuthor(const char* newAuthor);
	void setWeight(const double newWeight);

	const char* getTitle() const;
	const char* getAuthor() const;
	double getWeight() const;
	ISBN getISBN() const;

	void print() const;

private:
	char* title;
	char* author;
	double weight;
	ISBN isbn;
};

#endif