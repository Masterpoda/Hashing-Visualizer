#include <iostream>
#include <fstream>

using namespace std;

int main(){

ifstream fin, fin2;
ofstream fout;
long long holder, compare = 3;

fin.open("firstmil.txt");
fout.open("tablesizes.txt");

    while(fin)
    {
    fin >> holder;
    if(holder >= compare*2)
        {
            fout<<holder<<endl;
            compare = holder;
        }
    }

    fin.close();
    fin2.open("primes2.txt");

    while(fin2)
    {
        fin2>>holder;
        if(holder >= compare*2)
            {
                fout<<holder<<endl;
                compare = holder;
            }
    }

    fin.close();
    fin.open("primes4.txt");

    while(fin)
    {
        fin >> holder;
        if(holder >= compare*2)
            {
                fout<<holder<<endl;
                compare = holder;
            }
    }


    fin.close();
    fin.open("primes7.txt");

    while(fin)
    {
        fin >> holder;
        if(holder >= compare*2)
            {
                fout<<holder<<endl;
                compare = holder;
            }
    }

    fin.close();
    fin.open("primes14.txt");

    while(fin)
    {
        fin >> holder;
        if(holder >= compare*2)
            {
                fout<<holder<<endl;
                compare = holder;
            }
    }

    fin.close();
    fin.open("primes27.txt");

    while(fin)
    {
        fin >> holder;
        if(holder >= compare*2)
            {
                fout<<holder<<endl;
                compare = holder;
            }
    }

    fin.close();
    fin.open("primes50.txt");
    while(fin)
    {
        fin >> holder;
        if(holder >= compare*2)
            {
                fout<<holder<<endl;
                compare = holder;
            }
    }




    fout.close();
    return 0;
}
