#include "ISBN.h"
#include "Book.h"
#include "Backpack.h" 

#include <iostream>
#include <fstream>

constexpr size_t commandLen = 10;
constexpr char fileName[] = "BackpackBin.bin";

enum class Command
{
	Add,
	Remove,
	Transfer,
	Restore,
	Print,
	Close,
	Invalid
};

Command getCommand(const char* input)
{
	if (strcmp(input, "add") == 0)
	{
		return Command::Add;
	}
	if (strcmp(input, "remove") == 0)
	{
		return Command::Remove;
	}
	if (strcmp(input, "transfer") == 0)
	{
		return Command::Transfer;
	}
	if (strcmp(input, "restore") == 0)
	{
		return Command::Restore;
	}
	if (strcmp(input, "print") == 0)
	{
		return Command::Print;
	}
	if (strcmp(input, "close") == 0)
	{
		return Command::Close;
	}
	return Command::Invalid;
}

bool inputBookInfo(Book& book)
{
	char title[MAX_TEXT_LEN + 1];
	char author[MAX_TEXT_LEN + 1];
	double weight;
	unsigned isbn[ISBNLen];

	std::cout << "Enter book title: \n";
	std::cin.getline(title, MAX_TEXT_LEN + 1);

	std::cout << "Enter author: \n";
	std::cin.getline(author, MAX_TEXT_LEN + 1);

	std::cout << "Enter weight: \n";
	std::cin >> weight;
	std::cin.ignore();

	std::cout << "Enter ISBN: \n";
	for(unsigned i = 0; i < ISBNLen; i++)
	{
		std::cin >> isbn[i];
	}		
	std::cin.ignore();

	try
	{
		ISBN isbnBook(isbn);
		book = Book(title, author, weight, isbnBook);
	}
	catch (const std::invalid_argument& e)
	{
		std::cout << e.what();
		return false;
	}
	catch (const std::bad_alloc& e)
	{
		std::cout << e.what();
		return false;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
		return false;
	}

	return true;
}

int main()
{
	char input[commandLen];
	Command command;
	Backpack backpack;
	bool goOn = true;

	do 
	{
		std::cout << "Enter command: \n";
		std::cin >> input;
		std::cin.ignore();

		command = getCommand(input);

		if (command == Command::Add)
		{
			
			Book book;
			if (!inputBookInfo(book))
			{
				return 1;
			}
			try
			{
				backpack.addBook(book);
			}
			catch (const std::exception e)
			{
				std::cout << e.what();
				return 1;
			}
		
			std::cout << "Successfully added book \n";
			goOn = true;
		}
		else if (command == Command::Remove)
		{
			goOn = true;
			std::cout << "Enter ISBN: \n";
			unsigned isbn[ISBNLen];

			for (unsigned i = 0; i < ISBNLen; i++)
			{
				std::cin >> isbn[i];
			}
			std::cin.ignore();

			try
			{
				ISBN isbnBook(isbn);
				backpack.removeBook(isbnBook);
			}
			catch (const std::exception& e)
			{
				std::cout << e.what();
				return 1;
			}
		
			std::cout << "Successfully removed book\n";
		}
		else if (command == Command::Transfer)
		{
			goOn = true;
			backpack.transferToBinary(fileName);
		}
		else if (command == Command::Restore)
		{
			goOn = true;
			backpack = backpack.restore(fileName);
		}
		else if (command == Command::Print)
		{
			goOn = true;
			backpack.print();
		}
		else if(command == Command::Close)
		{
			goOn = false;
		}
		else
		{
			std::cout << "Add, Remove, Transfer, Restore, Print \n";
			goOn = true;
		}
	} while (goOn);

	return 0;
}
