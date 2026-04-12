#define _CRT_SECURE_NO_WARNINGS

#include "Book.h"
#include <iostream>
#include <stdexcept>

Book::Book(char* newTitle, char* newAuthor, float newWeight, ISBN newISBN)
	:Book()
{
	if (!newTitle || !newAuthor || newWeight == 0.0)
	{
		throw std::invalid_argument("Invalid book data");
	}

	title = new char[std::strlen(newTitle) + 1];
	strcpy(title, newTitle);

	try
	{
		author = new char[strlen(newAuthor) + 1];
	}
	catch (std::bad_alloc&)
	{
		delete[] title;
		title = nullptr;
		throw;
	}
	strcpy(author, newAuthor);

	weight = newWeight;
	isbn = newISBN;
}

Book::Book() : title(nullptr), author(nullptr), weight(0.0), isbn()
{ }

Book::Book(const Book& other)
	: Book()
{
	title = new char[std::strlen(other.title) + 1];
	strcpy(title, other.title);

	try
	{
		author = new char[std::strlen(other.author) + 1];
	}
	catch (std::bad_alloc&)
	{
		delete[] title;
		title = nullptr;
		throw;
	}
	strcpy(author, other.author);

	weight = other.weight;
	isbn = other.isbn;
}

Book& Book::operator=(const Book& other)
{
	if (this != &other)
	{
		char* newTitle = new char[strlen(other.title) + 1];
		strcpy(newTitle, other.title);

		char* newAuthor = nullptr;

		try
		{
			newAuthor = new char[strlen(other.author) + 1];
		}
		catch (std::bad_alloc&)
		{
			delete[] newTitle;
			throw; 
		}
		strcpy(newAuthor, other.author);

		delete[] title;
		delete[] author;

		title = newTitle;
		author = newAuthor;
		weight = other.weight;
		isbn = other.isbn;
	}
	return *this;
}

Book::~Book()
{
	delete[] title;
	delete[] author;
}

void Book::setTitle(const char* newTitle)
{
	if (newTitle == nullptr)
	{
		throw std::invalid_argument("Title is empty!\n");
	}

	char* tempTitle = new char[strlen(newTitle) + 1];
	tempTitle[strlen(newTitle)] = '\0';
	strcpy(tempTitle, newTitle);

	delete[] title;

	title = tempTitle;
}

void Book::setAuthor(const char* newAuthor)
{
	if (newAuthor == nullptr)
	{
		throw std::invalid_argument("No author name!\n");
	}
	char* tempAuthor = new char[strlen(newAuthor) + 1];
	tempAuthor[strlen(newAuthor)] = '\0';
	strcpy(tempAuthor, newAuthor);

	delete[] author;

	author = tempAuthor;
}

void Book::setWeight(const double newWeight)
{
	if (newWeight <= 0.0)
	{
		throw std::invalid_argument("Invalid weight entered!\n");
	}

	weight = newWeight;
}

const char* Book::getTitle() const
{
	return title;
}

const char* Book::getAuthor() const
{
	return author;
}

double Book::getWeight() const
{
	return weight;
}

ISBN Book::getISBN() const
{
	return isbn;
}

void Book::print() const
{
	std::cout << "Title: " << title << "\n" <<
		"Author: " << author << "\n" <<
		"Weight: " << weight << "\n";
	isbn.print();
}