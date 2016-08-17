#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include "hash.h"

using namespace std;

int main(){
    Hash<int, int> testTable(5);
    string key;
    ifstream fin;
    int data = 1;

    fin.open("2600.txt");

    cout<<"Beginning intense insertion action"<<endl;

    int i = 0;
    testTable.remove(i);

    for(int i = 1; i < 100; i++)
    {
        testTable.insert(i,i);
    }

    for(int i = 1; i < 1000; i++)
    {
        testTable.remove(i);
    }

    cout<<fixed<<setprecision(10)<<endl;
    cout<<"contents: "<<testTable.contents()<<endl;
    cout<<"contents: "<<testTable.loadFactor()<<endl;

    fin.close();

    return 0;
}
