
#include <fstream>
#include <vector>
#include "serializer.h"
//#include <externs.h>


void serializer::loadData(std::fstream *stream)
{
	saveLoadHandler(stream, true);
}

void serializer::saveData(std::fstream *stream)
{
	saveLoadHandler(stream, false);
}

void serializer::saveLoadHandler(std::fstream *stream, bool reading)
{
}

void serializer::serializeHandler(std::fstream *stream, bool reading, char &data)
{
	if(reading)
	{
		stream->read(reinterpret_cast<char*>(&data), sizeof(data));
	}
	else
	{
		stream->write(reinterpret_cast<char*>(&data), sizeof(data));
	}
}

void serializer::serializeHandler(std::fstream *stream, bool reading, short &data)
{
	if(reading)
	{
		stream->read(reinterpret_cast<char*>(&data), sizeof(data));
	}
	else
	{
		stream->write(reinterpret_cast<char*>(&data), sizeof(data));
	}
}

void serializer::serializeHandler(std::fstream *stream, bool reading, int &data)
{
	if(reading)
	{
		stream->read(reinterpret_cast<char*>(&data), sizeof(data));
	}
	else
	{
		stream->write(reinterpret_cast<char*>(&data), sizeof(data));
	}
}

void serializer::serializeHandler(std::fstream *stream, bool reading, float &data)
{
	if(reading)
	{
		stream->read(reinterpret_cast<char*>(&data), sizeof(data));
	}
	else
	{
		stream->write(reinterpret_cast<char*>(&data), sizeof(data));
	}
}

void serializer::serializeHandler(std::fstream *stream, bool reading, std::string &data)
{
	unsigned int size = 0;
	if(reading)
	{
		//fread(&size, sizeof(int), 1, fpt);
		stream->read(reinterpret_cast<char*>(&size), sizeof(size));
		char *tempString;
		tempString = new char[size+1];
		stream->read(tempString, size);
		tempString[size] = 0;
		data = tempString;
		delete [] tempString;
	}
	else
	{
		size = (unsigned int)data.size();
		//fwrite(&size, sizeof(int), 1, fpt);
		stream->write(reinterpret_cast<char*>(&size), sizeof(size));
		stream->write(data.c_str(), size);
	}
}

void serializer::serializeHandler(std::fstream *stream, bool reading, serializer &data)
{
	data.saveLoadHandler(stream, reading);
}