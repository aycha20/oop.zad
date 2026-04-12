#include "ISBN.h"
#include "Book.h"
#include "Backpack.h" 

#include <iostream>
#include <fstream>

int main()
{
	Backpack backpack;

	unsigned isbn1[] = { 9,7,8,1,4,7,6,7,3,8,0,2,4 };
	ISBN isbn(isbn1);

	char title[] = "A man called Ove";
	char author[] = "Fredrik Backman";
	Book book1(title, author, 0.7, isbn);

	backpack.addBook(book1);

	backpack.transferToBinary("BackpackBin.bin");

	backpack.print();

	backpack.removeBook(isbn);

	backpack.print();

	backpack = backpack.restore("BackpackBin.bin");

	backpack.print();


	return 0;
}