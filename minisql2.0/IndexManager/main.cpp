#include <iostream>
//#include "Interpreter.hpp"
//#include "CatalogManager.h"
//#include "RecordManager.h"
#include "IndexManager.h"
//#include "BufferManager.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#include "API.h"




int main(int argc,char * argv[])
{
	IndexManager im;
	string indexName = "im.txt";
	im.CreateIndex(indexName, 4, 0);
	im.InsertIntoIndex(indexName, "1", 0, 1);
	im.InsertIntoIndex(indexName, "2", 0, 2);
	im.InsertIntoIndex(indexName, "3", 0, 2);
	im.InsertIntoIndex(indexName, "4", 0, 2);
	im.InsertIntoIndex(indexName, "5", 0, 2);
	im.InsertIntoIndex(indexName, "6", 0, 2);
	im.InsertIntoIndex(indexName, "7", 0, 7);
	im.InsertIntoIndex(indexName, "8", 0, 2);
	im.InsertIntoIndex(indexName, "9", 0, 2);
	im.InsertIntoIndex(indexName, "10", 0, 10);
	im.InsertIntoIndex(indexName, "11", 0, 2);
	im.InsertIntoIndex(indexName, "12", 0, 2);

	im.levelListIndex(indexName);

	im.DeleteInIndex(indexName, "9", 0);
	im.DeleteInIndex(indexName, "8", 0);
	im.levelListIndex(indexName);
	int temp = im.SearchInIndex(indexName, "10", 0);
	cout << temp << endl;
	temp = im.SearchInIndex(indexName, "7", 0);
	cout << temp << endl;
	system("pause");
    
    return 0;
}

