#include "Buffer.h"

int Buffer::used_byte = 0;
char Buffer::result[50000000] = { '\0' };


Buffer::Buffer()
{
	/*used_byte = 0;
	result = (char *)malloc(sizeof(char) * OffsetByte * 500);
	memset(result, '\0', sizeof(char) * OffsetByte * 500);*/
}
Buffer::~Buffer()
{

}
char* Buffer::Get_Node(OffsetType offset)
{
	char *Content = (char*)malloc(sizeof(char) * OffsetByte);
	int begin = OffsetByte * offset;
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
void Buffer::Update_Node(OffsetType offset, char * Content)
{

	int begin = OffsetByte * offset;
	int end = begin + OffsetByte;
	if (used_byte >= end)
	{
		for (int i = begin; i < end; ++i)
		{
			result[i] = Content[i % (OffsetByte)];

		}
	}
	else
	{
		cout << "save node error" << endl;
	}

}
OffsetType Buffer::Create_Node()
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

void Buffer::Delete_Node(OffsetType offset)
{
	int begin = OffsetByte * offset;
	int end = begin + OffsetByte;
	for (int i = begin; i < end; ++i)
	{
		result[i] = '\0';

	}
}


char* Buffer::GetIndexHead()
{
	char *Content = Get_Node(0);
	return Content;
}

void Buffer::UpdateIndex(char * Content)
{
	Update_Node(0, Content);
}