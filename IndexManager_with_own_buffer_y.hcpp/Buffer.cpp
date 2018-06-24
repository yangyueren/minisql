#include "Buffer.h"

int BufferManager::used_byte = 0;
char BufferManager::result[50000000] = { '\0' };
BufferManager bm;

BufferManager::BufferManager()
{
	/*used_byte = 0;
	result = (char *)malloc(sizeof(char) * OffsetByte * 500);
	memset(result, '\0', sizeof(char) * OffsetByte * 500);*/
}
BufferManager::~BufferManager()
{

}

char* BufferManager::getBlockByOffset(string fileName, int offsetInFile)
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
void BufferManager::updateBlock(string fileName, char *UpdateContent, int offsetInFile)
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
void BufferManager::deleteBlock(string fileName, int offsetInFile)
{
	int begin = OffsetByte * offsetInFile;
	int end = begin + OffsetByte;
	for (int i = begin; i < end; ++i)
	{
		result[i] = '\0';

	}
}
int BufferManager::createNewBlock(string fileName)
{
	int curLength = used_byte;
	int newLength = curLength + OffsetByte;
	if (curLength % OffsetByte == 0)
	{
		used_byte = newLength;
	}
	for (int i = 0; i < OffsetByte; ++i)
	{
		result[curLength + i] = ' ';
	}
	return (newLength / OffsetByte - 1);
}
//
//
//
//char* BufferManager::Get_Node(OffsetType offset)
//{
//	char *Content = (char*)malloc(sizeof(char) * OffsetByte);
//	int begin = OffsetByte * offset;
//	int end = begin + OffsetByte;
//	if (used_byte >= end)
//	{
//		for (int i = begin; i < end; ++i)
//		{
//			Content[i%OffsetByte] = result[i];
//		}
//	}
//	else
//	{
//		cout << "get node error" << endl;
//	}
//
//	return Content;
//}
//void BufferManager::Update_Node(OffsetType offset, char * Content)
//{
//
//	int begin = OffsetByte * offset;
//	int end = begin + OffsetByte;
//	if (used_byte >= end)
//	{
//		for (int i = begin; i < end; ++i)
//		{
//			result[i] = Content[i % (OffsetByte)];
//
//		}
//	}
//	else
//	{
//		cout << "save node error" << endl;
//	}
//
//}
//OffsetType BufferManager::Create_Node()
//{
//	int curLength = used_byte;
//	int newLength = curLength + OffsetByte;
//	if (curLength % OffsetByte == 0)
//	{
//		used_byte = newLength;
//	}
//	for (int i = 0; i < OffsetByte; ++i)
//	{
//		result[curLength + i] = ' ';
//	}
//	return (newLength / OffsetByte - 1);
//}
//
//void BufferManager::Delete_Node(OffsetType offset)
//{
//	int begin = OffsetByte * offset;
//	int end = begin + OffsetByte;
//	for (int i = begin; i < end; ++i)
//	{
//		result[i] = '\0';
//
//	}
//}
//
//
//char* BufferManager::GetIndexHead()
//{
//	char *Content = Get_Node(0);
//	return Content;
//}
//
//void BufferManager::UpdateIndex(char * Content)
//{
//	Update_Node(0, Content);
//}