#include <iostream>
//#include "Interpreter.hpp"
//#include "CatalogManager.h"
//#include "RecordManager.h"
#include "IndexManager.h"
//#include "BufferManager_y.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#include "API.h"




int main(int argc,char * argv[])
{

	string indexName = "im3.txt";
	vector<int> re;

	//im.CreateIndex("im1.txt", 4, 0);
	//im.CreateIndex("im3.txt", 4, 0);
	//im.InsertIntoIndex("im2.txt", "1", 0, 1);
	//im.InsertIntoIndex(indexName, "2", 0, 2);
	//im.InsertIntoIndex("im2.txt", "3", 0, 2);
	//im.InsertIntoIndex(indexName, "4", 0, 2);
	im.InsertIntoIndex(indexName, "5", 0, 2);
	im.InsertIntoIndex(indexName, "6", 0, 2);
	im.InsertIntoIndex(indexName, "7", 0, 7);
	im.InsertIntoIndex(indexName, "8", 0, 2);
	im.InsertIntoIndex(indexName, "9", 0, 2);
	im.InsertIntoIndex(indexName, "10", 0, 10);
	im.InsertIntoIndex(indexName, "11", 0, 2);
	im.InsertIntoIndex(indexName, "12", 0, 2);
	im.levelListIndex(indexName, 0);
	im.SearchInRange(indexName, 0, "10", 1, "9", 1, re);
	cout << re.at(0) << endl;
	im.DeleteInIndex(indexName, "9", 0);
	im.DeleteInIndex(indexName, "8", 0);
	im.levelListIndex(indexName, 0);
	im.writeToDisk();
	int temp = im.SearchInIndex(indexName, "10", 0);
	cout << temp << endl;
	temp = im.SearchInIndex(indexName, "7", 0);
	cout << temp << endl;
	system("pause");
    
    return 0;
}

