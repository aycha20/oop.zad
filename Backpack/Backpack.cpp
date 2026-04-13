#include "Backpack.h"
#include "ISBN.h"

#include <iostream>
#include <fstream>

const double Backpack::maxWeight = 6.5;

Backpack::Backpack()
	: books(nullptr), currentWeight(0.0), bookCount(0)
{}

Backpack::Backpack(const Backpack& other)
	:Backpack()
{
	Book* temp = new Book[other.bookCount];

	for (unsigned i = 0; i < other.bookCount; i++)
	{
		temp[i] = other.books[i];
	}

	books = temp;
	currentWeight = other.currentWeight;
	bookCount = other.bookCount;

}

Backpack& Backpack::operator=(const Backpack& other)
{
	if (this != &other)
	{
		Book* newBooks = new Book[other.bookCount];

		for (unsigned i = 0; i < other.bookCount; i++)
		{
			newBooks[i] = other.books[i];
		}

		delete[] books;

		books = newBooks;
		currentWeight = other.currentWeight;
		bookCount = other.bookCount;
	}
	return *this;
}

Backpack::~Backpack()
{
	delete[] books;
}

const unsigned Backpack::getBookCount() const
{
	return bookCount;
}

void Backpack::addBook(Book& newBook)
{
	if (currentWeight + newBook.getWeight() > maxWeight)
	{
		throw std::runtime_error("Weight passed limit!\n");
	}

	Book* temp = new Book[getBookCount() + 1];
	
	for (unsigned i = 0; i < bookCount; i++)
	{
		temp[i] = books[i];
	}
	
	temp[getBookCount()] = newBook;

	
	delete[] books;

	books = temp;
	bookCount++;

	currentWeight += newBook.getWeight();
}

bool Backpack::removeBook(ISBN& InputISBN)
{
	for (unsigned i = 0; i < bookCount; i++)
	{
		if (books[i].getISBN().compareISBN(InputISBN))
		{
			currentWeight -= books[i].getWeight();

			for (unsigned j = i; j < bookCount - 1; j++)
			{
				books[j] = books[j + 1];
			}

			Book* temp = new Book[bookCount - 1];

			for (unsigned j = 0; j < bookCount - 1; j++)
			{
				temp[j] = books[j];
			}
			
			delete[] books;
			books = temp;
			bookCount --;
			
			return true;
		}
	}
	return false;
}

void Backpack::transferToBinary(const char* fileName)
{
	std::ofstream file (fileName, std::ios::binary | std::ios::app);
	if (!file.is_open())
	{
		throw "File cannot open!\n";
	}

	file.write(reinterpret_cast<const char*>(&bookCount), sizeof(bookCount));

	for (unsigned i = 0; i < bookCount; i++)
	{
		size_t titleLen = strlen(books[i].getTitle());

		file.write(reinterpret_cast<const char*>(&titleLen), sizeof(titleLen));
		file.write(books[i].getTitle(), titleLen);

		size_t authorLen = strlen(books[i].getAuthor());
		file.write(reinterpret_cast<const char*>(&authorLen), sizeof(authorLen));
		file.write(books[i].getAuthor(), authorLen);

		double weight = books[i].getWeight();

		file.write(reinterpret_cast<const char*>(&weight), sizeof(weight));

		file.write(reinterpret_cast<const char*>(books[i].getISBN().getISBN()), sizeof(unsigned) * ISBNLen);
	}
	file.close();
}

Backpack Backpack::restore(const char* fileName)
{
	std::ifstream file(fileName, std::ios::binary);
	if (!file.is_open())
	{
		throw "File cannot be opened!\n";
	}

	Backpack result;
	unsigned count = 0;

	file.read(reinterpret_cast<char*>(&count), sizeof(count));
	/*if (!file)
	{
		throw std::runtime_error("Failed to read book count.");
	}*/
	result.books = new Book[count];
	result.bookCount = count;
	result.currentWeight = 0;

	for (unsigned i = 0; i < count; i++)
	{
		size_t titleLen = 0;
		file.read(reinterpret_cast<char*>(&titleLen), sizeof(titleLen));

		char* newTitle = new char[titleLen + 1];
		file.read(newTitle, titleLen);
		newTitle[titleLen] = '\0';

		size_t authorLen = 0;
		file.read(reinterpret_cast<char*>(&authorLen), sizeof(authorLen));

		char* newAuthor = nullptr;

		try
		{
			newAuthor = new char[authorLen + 1];
		}
		catch (std::bad_alloc)
		{
			delete[] newTitle;//taka moje li?
			newTitle = nullptr;
		}
		
		file.read(newAuthor, authorLen);
		newAuthor[authorLen] = '\0';

		double weigh = 0.0;
		file.read(reinterpret_cast<char*>(&weigh), sizeof(weigh));
		if (!file)
		{
			delete[] newTitle;
			delete[] newAuthor;
			throw std::runtime_error("Failed to read weight.");
		}

		unsigned isbnArr[ISBNLen];
		file.read(reinterpret_cast<char*>(isbnArr), sizeof(unsigned) * ISBNLen);
		if (!file)
		{
			delete[] newTitle;
			delete[] newAuthor;
			throw std::runtime_error("Failed to read ISBN.");
		}

		result.books[i] = Book(newTitle, newAuthor, weigh, ISBN(isbnArr));
		result.currentWeight += weigh;

		delete[] newTitle;
		delete[] newAuthor;
	}
	return result;
}

void Backpack::print() const
{
	for (unsigned i = 0; i < bookCount; i++)
	{
		books[i].print();
	}
	std::cout << "\n";
}
