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
    API api;
    //index_manager.CreateIndex("2", 15, 2, 15);
    //index_manager.CreateIndex("IndexFile_stunameidx", 16, 2, 16);
    //index_manager.InsertIntoIndex("IndexFile_stunameidx", "wy", 2, 2);
    //index_manager.InsertIntoIndex("IndexFile_stunameidx", "wyz", 2, 2);
    //index_manager.InsertIntoIndex("IndexFile_stunameidx", "wzy", 2, 2);
    //cout << index_manager.SearchInIndex("IndexFIle_primary_key_on_student", "1234678", 2) << endl;
    //index_manager.levelListIndex("IndexFIle_primary_key_on_student", 2);
    
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
    return 0;
}
