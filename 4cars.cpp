#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <cstring>

constexpr size_t NAME_LEN = 23;
constexpr size_t INPUT_SIZE = 16;
constexpr const char* FILE_NAME = "db-save.dat";
constexpr size_t MAX_REGISTR = 100;
constexpr size_t MODEL_COUNT = 4;

constexpr unsigned CAR_POWER[MODEL_COUNT] = {
	670,    //  0 - Lamborghini Murcielago
	503,    //  1 - Mercedes AMG
	740,    //  2 - Pagani Zonda R
	1020    //  3 - Bugatti Veyron
};

constexpr const char* REPORT_NAME = "car-report.txt";

const char* getCarName(unsigned id)
{
	switch (id)
	{
	case 0: return "Lamborghini Murcielago";
	case 1: return "Mercedes AMG";
	case 2: return "Pagani Zonda R";
	case 3: return "Bugatti Veyron";
	default: return "Unknown";
	}
}

enum class Command
{
	New,
	Save,
	Info,
	Exit,
	Generate_Report,
	Invalid
};

Command translCommand(char* input)
{
	if (strcmp(input, "info") == 0)
	{
		return Command::Info;
	}
	else if (strcmp(input, "save") == 0)
	{
		return Command::Save;
	}
	else if (strcmp(input, "new") == 0)
	{
		return Command::New;
	}
	else if (strcmp(input, "exit") == 0)
	{
		return Command::Exit;
	}
	else if (strcmp(input, "report") == 0)
	{
		return Command::Generate_Report;
	}
	return Command::Invalid;
}

Command getCommand()
{
	char input[INPUT_SIZE];
	std::cout << "Enter command:" << std::endl;
	std::cin >> input;
	std::cin.ignore();

	return translCommand(input);
}

struct Data
{
	char name[NAME_LEN];
	unsigned id;
	unsigned registration;
};

void getData(Data& d)
{
	char temp[NAME_LEN];

	std::cout << "Enter name of owner: " << std::endl;
	std::cin.getline(temp, NAME_LEN);
	std::strncpy(d.name, temp, NAME_LEN - 1);
	d.name[NAME_LEN - 1] = '\0';

	do
	{
		std::cout << "Enter registration of car (4 digits): " << std::endl;
		std::cin >> d.registration;
		std::cin.ignore();
	} while (d.registration < 1000 || d.registration > 9999);
		
	do
	{
		std::cout << "Enter id of car: " << std::endl;
		std::cin >> d.id;
		std::cin.ignore();
	} while (d.id >= MODEL_COUNT);
}

bool saveToFile(Data& d)
{
	std::ofstream ofs(FILE_NAME, std::ios::binary | std::ios::app);

	if (!ofs.is_open())
	{
		std::cerr << "Can't open file!\n";
		return false;
	}

	ofs.write(reinterpret_cast<const char*>(&d), sizeof(Data));

	ofs.close();
	return true;
}

bool printData()
{
	std::ifstream ifs(FILE_NAME, std::ios::binary);
	if (!ifs)
	{
		std::cerr << "Empty\n";
		return false;
	}

	Data d;
	int idx = 0;

	while (ifs.read(reinterpret_cast<char*>(&d), sizeof(Data)))
	{
		std::cout << "/" << "\n" << d.name << "\n" << d.registration
			<< "\n" << getCarName(d.id) << std::endl;
		idx++;
	}
	if (idx == 0)
	{
		std::cout << "Empty\n";
	}
	ifs.close();
	return true;
}

bool loadFile(const char* fileName)
{
	std::ofstream ofs(fileName, std::ios::binary | std::ios::app);
	if (!ofs.is_open())
	{
		std::cerr << "Error opening file db-save.dat\n";
		return false;
	}
	return true;
}

int getRegNums(unsigned* regsArr)
{
	std::ifstream ifs(FILE_NAME, std::ios::binary);
	if (!ifs.is_open())
	{
		std::cerr << "Can't open file\n";
		return -1;
	}

	Data d;
	int count = 0;

	while (count < MAX_REGISTR && 
		ifs.read(reinterpret_cast<char*>(&d), sizeof(Data)))
	{
		regsArr[count++] = d.registration;
	}
	return count;
}

bool registrationInUse(unsigned reg, const unsigned* regArr, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (regArr[i] == reg)
		{
			return true;
		}
	}
	return false;
}

bool mostPopularCarNum(unsigned* carCount)
{
	std::ifstream ifs(FILE_NAME, std::ios::binary);
	if (!ifs.is_open())
	{
		std::cerr << "Can't open file\n";
		return false;
	}

	for (size_t i = 0; i < MODEL_COUNT; i++)
	{
		carCount[i] = 0;
	}

	Data d;

	while (ifs.read(reinterpret_cast<char*>(&d), sizeof(Data)))
	{
		if (d.id < MODEL_COUNT)
		{
			carCount[d.id] += 1;
		}
	}

	return true;
}

unsigned mostPopIdx(const unsigned* carCount)
{
	unsigned maxIdx = 0;
	unsigned maxCount = carCount[0];

	for (size_t i = 1; i < MODEL_COUNT; i++)
	{
		if (carCount[i] > maxCount)
		{
			maxCount = carCount[i];
			maxIdx = i;
		}
	}
	return maxIdx;
}

double getAveragePower()
{
	double avg = 0.0;

	std::ifstream ifs(FILE_NAME, std::ios::binary);
	if (!ifs.is_open())
	{
		std::cerr << "Can't open file\n";
		return -1;
	}

	Data d;
	unsigned totalSum = 0;
	int count = 0;

	while (ifs.read(reinterpret_cast<char*>(&d), sizeof(Data)))
	{
		if (d.id < MODEL_COUNT)
		{
			totalSum += CAR_POWER[d.id];
			count++;
		}
	}

	if (count == 0)
	{
		std::cerr << "File is empty\n";
		return -1;
	}

	avg = static_cast<double>(totalSum) / count;
	return avg;
}


bool generateTxt(const unsigned* carCount)
{
	unsigned mostPopCar = mostPopIdx(carCount);
	double averagePower = getAveragePower();
	if (averagePower < 0)
	{
		std::cerr << "Error with average\n";
		return false;
	}
	char names[MAX_REGISTR][NAME_LEN];
	unsigned totalPower[MAX_REGISTR];
	int ownerCount = 0;

	std::ofstream ofs(REPORT_NAME);
	if (!ofs.is_open())
	{
		std::cerr << "Error creatiing report\n";
		return false;
	}
	
	ofs << "Most popular car: " << getCarName(mostPopCar)
		<< " owned by " << carCount[mostPopCar] << " people\n";

	ofs << "Average power: " << averagePower << std::endl;

	std::ifstream ifs(FILE_NAME, std::ios::binary);
	if (!ifs.is_open())
	{
		std::cerr << "Error opening\n";
		return false;
	}

	Data d;

	while (ifs.read(reinterpret_cast<char*>(&d), sizeof(Data)))
	{
		bool found = false;

		for (int i = 0; i < ownerCount; i++)
		{
			if (strcmp(names[i], d.name) == 0)
			{
				totalPower[i] += CAR_POWER[d.id];
				found = true;
				break;
			}
		}

		if (!found)
		{
			strncpy(names[ownerCount], d.name, NAME_LEN - 1);
			names[ownerCount][NAME_LEN - 1] = '\0';

			totalPower[ownerCount] = CAR_POWER[d.id];
			ownerCount++;
		}
	}
	
	ofs << "    OWNERS     \n";

	for (int i = 0; i < ownerCount; i++)
	{
		ofs << names[i] << " - " << totalPower[i] << std::endl;;
	}
	std::cout << "Report saved: " << REPORT_NAME << std::endl;
	return true;
}

int main()
{
	if (!loadFile(FILE_NAME))
	{
		return -1;
	}

	unsigned regsArr[MAX_REGISTR];

	int numOfRegs = getRegNums(regsArr);
	if (numOfRegs == -1)
	{
		return -1;
	}

	Data d;
	bool okay = true;
	bool hasData = false;
	bool duplicate = false;

	while (okay)
	{
		Command command = getCommand();
	
		switch (command)
		{
		case Command::New: 
		{
			duplicate = false;
			getData(d);
			if (registrationInUse(d.registration, regsArr, numOfRegs))
			{
				std::cout << "Registration is already in use\n";
				duplicate = true;
			}
			else
			{
				hasData = true;
			}
			break;
		}
		case Command::Save:
		{
			if (duplicate || !hasData)
			{
				std::cout << "Can not save!\n";
				break;
			}
			if (saveToFile(d))
			{
				if (numOfRegs < (int)MAX_REGISTR)
				{
					regsArr[numOfRegs++] = d.registration;
				}
				hasData = false;
			}
			break;
		}
		case Command::Info:
		{
			printData();
			break;
		}
		case Command::Exit:
		{
			okay = false;
			break;
		}
		case Command::Generate_Report:
		{
			unsigned countArr[MODEL_COUNT];
			if (!mostPopularCarNum(countArr))
			{
				return -1;
			}
			generateTxt(countArr);
			break;
		}
		case Command::Invalid:
		{
			std::cout << "Invalid command, must be (new, save, info, exit, report)" 
				<< std::endl;
			break;
		}
		}
	}
	
	return 0;
}
