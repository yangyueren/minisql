#include "Buffer_y_disk.h"
#include <fstream>

int used_byte = 0;
char BufferManager_y::result[bufferpool] = { '\0' };
BufferManager_y bm_y;

BufferManager_y::BufferManager_y()
{
	
}
BufferManager_y::~BufferManager_y()
{

}
void BufferManager_y::loadToArray(string fileName)
{
	used_byte = 0;
	string path = fileName;	
	std::ifstream f(fileName, std::ios::binary);
	char uc_var;
	int i = 0;
	while (!f.eof())
	{

		f.read((char*)&uc_var, 1);
		result[i++] = uc_var;
	}
	used_byte = i - 1;
	f.close();
	/*used_byte = 0;
	string path = fileName;
	fstream f(path, ios::in);
	char n;
	while (!f.eof())
	{
		
		f.get(n);
		result[used_byte++] = n;
	}
	
	f.close();*/
}
void BufferManager_y::writeBackDisk(string fileName)
{
	string path = fileName;
	std::ofstream f(fileName, std::ios::binary);
	int i = 0;
	char u_ch;		
	while (i < used_byte)
	{
		u_ch = result[i];
		f.write((char*)&u_ch, 1);
		i++;
	}
	f.close();
	used_byte = 0;
	/*string path = fileName;
	fstream f(path, ios::out);
	int i = 0;
	char n;
	while (i < used_byte)
	{
		n = result[i];
		f.put(n);
		i++;
	}
	f.close();
	used_byte = 0;*/
}
char* BufferManager_y::getBlockByOffset(string fileName, int offsetInFile)
{
	char *Content = (char*)malloc(sizeof(char) * OffsetByte);
	int begin = OffsetByte * offsetInFile;
	int end = begin + OffsetByte;
	if (used_byte >= end)
	{
		for (int i = begin; i < end; ++i)
		{
			Content[i%OffsetByte] = result[i];
		}
	}
	else
	{
		cout << "get node error" << endl;
	}

	return Content;
}
void BufferManager_y::updateBlock(string fileName, char *UpdateContent, int offsetInFile)
{
	int begin = OffsetByte * offsetInFile;
	int end = begin + OffsetByte;
	if (used_byte >= end)
	{
		for (int i = begin; i < end; ++i)
		{
			result[i] = UpdateContent[i % (OffsetByte)];

		}
	}
	else
	{
		cout << "save node error" << endl;
	}
}
void BufferManager_y::deleteBlock(string fileName, int offsetInFile)
{
	int begin = OffsetByte * offsetInFile;
	int end = begin + OffsetByte;
	for (int i = begin; i < end; ++i)
	{
		result[i] = '*';

	}
}
int BufferManager_y::createNewBlock(string fileName)
{
	int curLength = used_byte;
	int newLength = curLength + OffsetByte;
	if (curLength % OffsetByte == 0)
	{
		used_byte = newLength;
		
	}
	for (int i = 0; i < OffsetByte; ++i)
	{
		result[curLength + i] = '*';
	}

	
	return (newLength / OffsetByte - 1);
}
