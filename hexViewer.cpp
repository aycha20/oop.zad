#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

constexpr size_t MAX_FILE_NAME = 50;
constexpr size_t MAX_COMMAND_LEN = 8;

enum class Command
{
	View,
	Add,
	Change,
	Save,
	SaveAs,
	Invalid
};

Command getCommand(const char* input)
{
	Command command;
	if (strcmp(input, "view") == 0)
	{
		command = Command::View;
	}
	else if (strcmp(input, "add") == 0)
	{
		command = Command::Add;
	}
	else if (strcmp(input, "change") == 0)
	{
		command = Command::Change;
	}
	else if (strcmp(input, "save") == 0)
	{
		command = Command::Save;
	}
	else if (strcmp(input, "save as") == 0)
	{
		command = Command::SaveAs;
	}
	else
	{
		command = Command::Invalid;
	}

	return command;
}

bool loadFile(std::ifstream& ifs, char* name)
{
	std::cout << "Enter file name: \n";
	std::cin.getline(name, MAX_FILE_NAME);

	ifs.open(name, std::ios::binary);
	if (!ifs.is_open())
	{
		std::cout << "Can't open file!" << std::endl;
		return false;
	}

	return true;
}

int getFileSize(std::ifstream& ifs, const char* name)
{
	if (!ifs.is_open())
	{
		std::cout << "File not opened!" << std::endl;
		return -1;
	}
	std::streampos curPos = ifs.tellg();

	ifs.seekg(std::streamoff(0), std::ios::end);

	std::streampos end = ifs.tellg();
	 
	int endP = static_cast<int>(end);

	ifs.seekg(curPos);

	return endP;
}

void readBin(const size_t fileSize, const unsigned char* fileInfo)
{
	for (size_t i = 0; i < fileSize; i++)
	{
		std::cout << std::hex << std::uppercase << static_cast<int>(fileInfo[i]) << " ";
	}
	std::cout << std::dec << std::nouppercase;
	std::cout << std::endl;
}

bool isValid(const char c)
{
	return (c >= 'A' || c <= 'Z') || (c >= 'a' || c <= 'z');
}

void getASCII(const size_t fileSize, const unsigned char* fileInfo)
{	
	for (size_t i = 0; i < fileSize; i++)
	{
		if (!isValid(fileInfo[i]))
		{
			std::cout << ".. ";
			continue;
		}
		std::cout << fileInfo[i] << " ";
	}
	std::cout << std::endl;
}

void loadArr(std::ifstream& ifs, unsigned char*& arr, const size_t size)
{
	ifs.read(reinterpret_cast<char*>(arr), size);
}

unsigned char* allocate(const size_t size)
{
	unsigned char* arr = new(std::nothrow) unsigned char[size];
	if (!arr)
	{
		std::cerr << "could not allocate memory\n";
		return nullptr;
	}
	return arr;
}

bool reallocate(unsigned char*& arr, size_t& size)
{
	size_t newSize = size + 1;
	
	unsigned char* newArr = new(std::nothrow) unsigned char[newSize];
	if (!newArr)
	{
		std::cerr << "No memory!\n";
		return false;
	}

	for (size_t i = 0; i < size; i++)
	{
		newArr[i] = arr[i];
	}

	delete[] arr;
	arr = newArr;
	size = newSize;
	return true;
}

bool addToEnd(unsigned char*& arr, size_t& size)
{
	if (!reallocate(arr, size))
	{
		std::cout << "Can not add symbol, allocation failure!\n";
		return false;
	}

	std::cout << "Enter byte to end:\n";
	unsigned int input = 0;	
	std::cin >> std::hex >> input;

	arr[size - 1] = static_cast<unsigned char>(input);

	std::cin >> std::dec;
	std::cin.ignore();
	return true;
}

bool changeByte(unsigned char*& arr, const size_t size)
{
	unsigned index = 0;
	unsigned int input = 0;

	std::cout << "Enter index and byte: " << std::endl;
	std::cin >> index >> std::hex >> input;

	if (index >= size)
	{
		std::cout << "Input error!\n";
		return false;
	}

	arr[index] = input;

	std::cin >> std::dec;
	std::cin.ignore();
	return true;
}

bool saveChanges(const char* fileName, const unsigned char* arr, const size_t size)
{
	std::ofstream ofs(fileName, std::ios::trunc | std::ios::binary);

	if (!ofs.is_open())
	{
		std::cerr << "Can't open file!\n";
		return false;
	}

	ofs.write(reinterpret_cast<const char*>(arr), size);
	ofs.close();
	return true;
}

bool loadOFile(std::ofstream& ofs, char* name)
{
	std::cout << "Enter file name: \n";
	std::cin.getline(name, MAX_FILE_NAME);

	ofs.open(name, std::ios::binary);
	if (!ofs.is_open())
	{
		std::cout << "Can't open file!" << std::endl;
		return false;
	}

	return true;
}

bool saveAsNewFile(const unsigned char* arr, const size_t size)
{
	std::ofstream ofs;
	char name[MAX_FILE_NAME];

	if(!loadOFile(ofs, name))
	{
		std::cout << "Fail!" << std::endl;
		return false;
	}

	ofs.write(reinterpret_cast<const char*>(arr), size);
	ofs.close();
	return true;
}

int main()
{
	std::ifstream ifs;
	char name[MAX_FILE_NAME];

	if (loadFile(ifs, name))
	{
		std::cout << "File loaded successfully!" << std::endl;
	}
	else
	{
		std::cout << "Loading error!\n";
		return -1;
	}

	int fileS = getFileSize(ifs, name);

	if (fileS == -1)
	{
		std::cout << "Error! \n";
		return -1;
	}

	size_t size = static_cast<size_t>(fileS);

	std::cout << "Size: " << size << " bytes" << std::endl;

	unsigned char* fileInfo = allocate(size);
	if (!fileInfo)
	{
		std::cerr << "Allocation failure!\n";
		return -1;
	}

	loadArr(ifs, fileInfo, size);

	ifs.close();

	Command command;
	char input[MAX_COMMAND_LEN];

	do
	{
		std::cout << "Enter command view / add / change / save / save as." << std::endl;
		
		std::cin.getline(input, MAX_COMMAND_LEN);
		command = getCommand(input);

		if (command == Command::View)
		{ 
			readBin(size, fileInfo);
			getASCII(size, fileInfo);
		}
		else if (command == Command::Add)
		{
			if (!addToEnd(fileInfo, size))
			{
				std::cout << "Error!" << std::endl;
				return -1;
			}
			std::cout << "Successfull execution!" << std::endl;
		}
		else if (command == Command::Change)
		{
			if (!changeByte(fileInfo, size))
			{
				std::cout << "Error!" << std::endl;
				return -1;
			}
			std::cout << "Successfull execution!" << std::endl;

		}
		else if (command == Command::Save)
		{
			if (!saveChanges(name, fileInfo, size))
			{
				std::cout << "Error!" << std::endl;
				return -1;
			}
			std::cout << "Successfully saved!" << std::endl;
			break;
		}
		else if (command == Command::SaveAs)
		{
			if (!saveAsNewFile(fileInfo, size))
			{
				std::cout << "Could not save as new file!\n";
				return - 1;
			}
			std::cout << "Successfully saved!" << std::endl;
			break;
		}

	} while (command != Command::Save && command != Command::SaveAs);
	
	delete[] fileInfo;
	return 0;
}
