#include <iostream>
#include "Interpreter.hpp"
#include "CatalogManager.h"
#include "RecordManager.h"
#include "IndexManager.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "API.h"


clock_t start = 0;
void init()
{
    FILE *fp;
    fp = fopen("Indexs", "r");
    if (fp == NULL)
    {
        fp = fopen("Indexs", "w+");
        return;
    }
    fclose(fp);
}

void print()
{
    clock_t finish = clock();
    double duration = (double)(finish - start) / CLOCKS_PER_SEC;
    duration *= 1000;
    printf( "now time is %2.1f milliseconds\n", duration * 1000);
}


int main(int argc,char * argv[])
{
    init();
    
    API api;
    

    clock_t finish;

    cout << "Welcome to the MiniSQL monitor. Commands end with ;" << endl;
    cout << "Copyright (c) 2018 bds & jjc & syj & yyr & zzy. All rights reserved." << endl;
    bool fileRead = false;
    //string fileName ="";
    ifstream file;
    Interpreter in;
    in.api = &api;
    string s;
    int status = 0;
    while(1)
    {
        if(fileRead == true)
        {

            file.open(in.file_name.c_str());
            if(!file.is_open())
            {
                cout<<"Fail to open "<<in.file_name<<endl;
                fileRead = false;
                continue;
            }
            while(getline(file,s,';'))
            {
                in.interpreter(s);
            }
            file.close();
            fileRead = false;
            finish = clock();
            double duration = (double)(finish - start) / CLOCKS_PER_SEC;
            duration *= 1000;
            printf( "%2.1f ms\n", duration );
        }
        else
        {

            cout<<"minisql> ";
            getline(cin,s,';');
            start = clock();
            status =  in.interpreter(s);
            if(status==2)
            {
                fileRead = true;
            }
            else if(status==-1)
            {
            	cout << "Bye" << endl;
                break;
            }
            else{
                finish = clock();
                double duration = (double)(finish - start) / CLOCKS_PER_SEC;
                duration *= 1000;
                printf( "The duration is %2.1f ms\n", duration);
            }
        }

    }
    
    return 0;
}

