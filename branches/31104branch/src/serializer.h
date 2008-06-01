

//This is the class that helps with serializing stuffs!
#ifndef SERIALIZER_H_INCLUDED
#define SERIALIZER_H_INCLUDED

//#include <vector>
//#include <fstream>

class serializer
{
public:
	void loadData(std::fstream *stream);
	void saveData(std::fstream *stream);

	virtual void saveLoadHandler(std::fstream *stream, bool reading);

	void serializeHandler(std::fstream *stream, bool reading, char &data);
	void serializeHandler(std::fstream *stream, bool reading, short &data);
	void serializeHandler(std::fstream *stream, bool reading, int &data);
	void serializeHandler(std::fstream *stream, bool reading, float &data);
	void serializeHandler(std::fstream *stream, bool reading, std::string &data);
	void serializeHandler(std::fstream *stream, bool reading, serializer &data);

	template <class T> void serializeVectorHandler(std::fstream *stream, bool reading, std::vector<T> &data)
	{
		unsigned int size = 0;
		if(reading)
		{
			//fread(&size, sizeof(int), 1, fpt);
			stream->read(reinterpret_cast<char*>(&size), sizeof(size));
			data.resize(size);
		}
		else
		{
			size = (unsigned int)data.size();
			//fwrite(&size, sizeof(int), 1, fpt);
			stream->write(reinterpret_cast<char*>(&size), sizeof(size));
		}
		for(int i = 0; i < size; i++)
		{
			serializeHandler(stream, reading, data[i]);
		}
	}
};

#endif