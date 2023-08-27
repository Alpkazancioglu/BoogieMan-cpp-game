#include "FileHandler.h"


void filehandler::initilizefile(const char* file_src)
{
	std::ofstream file_m(file_src);

	if (file_m.is_open())
	{
		file_m.close();
	}
	else
	{
		std::cout << "Unable write data to disk" << std::endl;
	}
}

void filehandler::WriteToFile(const char* file_src , int data_to_write , unsigned int *position)
{
	std::fstream file_m(file_src, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
	
	if (file_m.is_open())
	{
		std::string s = std::to_string(data_to_write);

		char const* pchar = s.c_str();

		file_m.seekg(*position, std::ios_base::beg);

		file_m << data_to_write << std::endl;
		
		*position += strlen(pchar)+1;

		file_m.close();

	}
	else
	{
		std::cout << "Unable to open the desired file to write!" << std::endl;
	}
}

void filehandler::WriteTextToFile(const char* file_src, const char* data, unsigned int* position)
{
	std::fstream file_m(file_src, std::ios_base::in | std::ios_base::out | std::ios_base::binary);

	if (file_m.is_open())
	{
		file_m.seekg(*position , std::ios_base::beg);

		file_m << data << std::endl;

		*position += strlen(data)+1;

		file_m.close();
		
	}
	else
	{
		std::cout << "Unable to open the desired file to write!" << std::endl;
		
	}
}

int filehandler::ReadFromFile(const char* file_src ,char *destination_string , int length)
{
	std::ifstream file_to_read_from;
	
	file_to_read_from.open(file_src , std::ios_base::in | std::ios_base::binary);

	if (file_to_read_from.is_open())
	{
		file_to_read_from.read(destination_string, (std::streamsize)length+1);

		file_to_read_from.close();

		return 0;
	}
	else
	{
		std::cout << "Unable to open the desired file to read!" << std::endl;

		return -1;
	}

}


