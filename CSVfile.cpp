#include <iostream>
#include <fstream>
#include <cstring>

constexpr size_t SIZE = 50;

struct Student
{	
	int fn;
	char firstName[SIZE];
	char lastName[SIZE];
	char email[SIZE];
};


void openFile(std::ifstream& file, char* arr)
{
	
	std::cout << "Open file: \n";
	std::cin >> arr;

	file.open()
	if (!file.is_open())
	{
		std::cerr << "Can't open file";
		return std::ifstream();
	}

	std::cout << "File successfully opened!" << std::endl;

	return file;
}

unsigned getCharCountFile(std::ifstream& ifs, char c)
{
	std::streampos currPos = ifs.tellg();
	ifs.seekg(0, std::ios::beg);

	unsigned count = 0;
	char curr;

	while (ifs.get(curr))
	{
		if (curr == c)
		{
			count++;
		}
	}

	ifs.clear();
	ifs.seekg(currPos);

	return count;
}

unsigned getLinesCount(const char* fileName)
{
	std::ifstream myFile(fileName);
	if (!myFile.is_open())
	{
		return 0;
	}

	myFile.seekg(0, std::ios::end);
	if (myFile.tellg() == 0)
	{
		return 0;
	}
	myFile.seekg(0);

	unsigned newLines = getCharCountFile(myFile, '\n');
	return newLines + 1;
}

Student* initStudArr(std::ifstream& ifs, const unsigned size, unsigned& count)
{
	if (size == 0)
	{
		count = 0;
		return nullptr;
	}

	Student* studArr = new(std::nothrow) Student[size];
	count = 0;

	while(count < size && ifs.getline(studArr[count].firstName, SIZE, ' '))
	{
		ifs.getline(studArr[count].lastName, SIZE, ',');
		ifs.getline(studArr[count].email, SIZE, ',');
		ifs >> studArr[count].fn;

		ifs.ignore();

		count++;
	}
	return studArr;
}

void printStudent(Student p)
{
	std::cout << "Name: " << p.firstName << " " << p.lastName << ", " <<
		"Email: " << p.email << ", " << "FN: " << p.fn << "\n" ;
}

int findStudent(Student* students, const unsigned size, const int facNum)
{
	for (unsigned i = 0; i < size; i++)
	{
		if (students[i].fn == facNum)
		{
			return i;
		}
	}
	return -1;
}


void printInfo(Student* students, const unsigned size, const int facNum)
{
	int studentNum = findStudent(students, size, facNum);

	if(studentNum == -1)
	{
		std::cout << "No student with that faculty number found!\n";
	}
	else
	{
		printStudent(students[studentNum]);
	}
}

void changeEmail(const char* fileName, Student* students, 
	const unsigned size, const unsigned facNum)
{
	int studentNum = findStudent(students, size, facNum);
	if (studentNum == -1)
	{
		return;
	}

	std::cout << "Enter new email \n";
	std::cin >> students[studentNum].email;

	std::ofstream file(fileName, std::ios::trunc);

	if (!file.is_open())
	{
		std::cerr << "Can't open file \n";
		return;
	}
	

	for (unsigned i = 0; i < size; i++)
	{
		file << students[i].firstName << " " << students[i].lastName << ","
			<< students[i].email << "," << students[i].fn << "\n";
	}
}

int main()
{
	char arr[SIZE];

	std::ifstream file = openFile(arr);

	unsigned size = getLinesCount(arr);

	unsigned sizeS = 0;

	Student* students = initStudArr(file, size, sizeS);

	int facNum = 0;
	char command[SIZE];

	do
	{
		std::cout << "Enter command print or edit and faculty number: \n";
		std::cin >> command;
		
		while (strcmp(command, "print") != 0 && strcmp(command, "edit") != 0 &&
			(strcmp(command, "save") != 0))
		{
			std::cout << "Invalid command!\n" << "try again: (edit, print, save): \n";
			std::cin >> command;
		}

		if (strcmp(command, "save") == 0)
		{
			break;
		}

		std::cin >> facNum;


		if (strcmp(command, "print") == 0)
		{
			printInfo(students, sizeS, facNum);
		}
		else if (strcmp(command, "edit") == 0)
		{
			changeEmail(arr, students, sizeS, facNum);
		}

	} while (strcmp(command, "save") != 0);

	std::cout << "File " << arr << " successfully saved! \n";
 

	delete[] students;

	return 0;
}
