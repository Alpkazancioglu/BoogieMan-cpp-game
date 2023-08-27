#pragma once
#include <iostream>
#include <fstream>
#include <raylib.h>
#include <string>



class filehandler
{
public:

	void initilizefile(const char* file_src);
	void WriteToFile(const char* file_src, int data_to_write, unsigned int* position);
	void WriteTextToFile(const char* file_src, const char* data, unsigned int* position);
	int ReadFromFile(const char* file_src, char* destination_string,int length);
	

private:

	

};



