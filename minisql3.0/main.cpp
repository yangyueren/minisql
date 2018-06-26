#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Interpreter.hpp"
#include "CatalogManager.h"
#include "RecordManager.h"
#include "IndexManager.h"
#include "API.h"
#define QUIT -1
#define EXEC_FILE 2

clock_t start = 0;
clock_t finish;

int main(int argc,char * argv[])
{
<<<<<<< HEAD

	string indexName = "im1.txt";
	vector<int> re;

	im.CreateIndex("im1.txt", 15, 1, 15);
	//im.CreateIndex("im3.txt", 4, 0);
	//im.InsertIntoIndex("im2.txt", "1", 0, 1);
	//im.InsertIntoIndex(indexName, "2", 0, 2);
	//im.InsertIntoIndex("im2.txt", "3", 0, 2);
	//im.InsertIntoIndex(indexName, "4", 0, 2);
	im.InsertIntoIndex(indexName, "5.6", 1, 2);
	im.InsertIntoIndex(indexName, "6.3", 1, 2);
	im.InsertIntoIndex(indexName, "7.2", 1, 7);
	im.InsertIntoIndex(indexName, "8", 1, 2);
	im.InsertIntoIndex(indexName, "9", 1, 2);
	im.InsertIntoIndex(indexName, "10", 1, 10);
	im.InsertIntoIndex(indexName, "11", 1, 2);
	im.InsertIntoIndex(indexName, "12", 1, 2);
	im.levelListIndex(indexName, 2);
	//im.SearchInRange(indexName, 0, "10", 1, "9", 1, re);
	//cout << re.at(0) << endl;
	im.DeleteInIndex(indexName, "9", 1);
	im.DeleteInIndex(indexName, "8", 1 );
	im.levelListIndex(indexName, 2);
	im.writeToDisk();
	int temp = im.SearchInIndex(indexName, "10", 2);
	cout << temp << endl;
	temp = im.SearchInIndex(indexName, "7", 2);
	cout << temp << endl;
	system("pause");
    
=======
    API api;
    index_manager.CreateIndex("2", 15, 2, 15);
    cout << "Welcome to the MiniSQL monitor. Commands end with ;" << endl;
    cout << "Copyright (c) 2018 bds & jjc & syj & yyr & zzy. All rights reserved." << endl;
    bool file_read = false;
    ifstream file;
    Interpreter interpreter;
    interpreter.api = &api;
    string s;
    int status = 0;
    while(1){
        if(file_read == false){
            cout<<"minisql> ";
            getline(cin, s, ';');
            start = clock();
            status =  interpreter.interpreter(s);
            if(status == EXEC_FILE){
                file_read = true;
            }
            else if(status == QUIT){
                cout << "Bye" << endl;
                break;
            }
            else{
                finish = clock();
                double duration = (double)(finish - start) / CLOCKS_PER_SEC;
                duration *= 1000;
                printf("The duration is %2.1f ms\n", duration);
            }
        }
        else{ // 读取文件
            file.open(interpreter.file_name.c_str());
            if(!file.is_open()){
                cout << "Fail to open " << interpreter.file_name <<endl;
                file_read = false;
                continue;
            }
            while(getline(file, s, ';')){
                interpreter.interpreter(s);
            }
            file.close();
            file_read = false;
            finish = clock();
            double duration = (double)(finish - start) / CLOCKS_PER_SEC;
            duration *= 1000;
            printf("The duration is %2.1f ms\n", duration);
        }
    }
>>>>>>> 4a3ff70c3f31151fbf549ec6ee440696a29a6f40
    return 0;
}
