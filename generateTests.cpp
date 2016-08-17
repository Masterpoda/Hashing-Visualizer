#include <iostream>
#include <ctime>
#include <string>
#include <cstdlib>

using namespace std;

typedef unsigned long long ull;

void genInts ( ull iteration )
{
	ull num;
	ull i;
	
	for ( i = 0; i < iteration; i++ )
	{
		num = rand ( );
		cout << num << " ";
	}
	

}

void genString ( int iteration )
{
	int len;
	string str;
	ull i, j;
	unsigned char temp;
	ull max;
	
	cin >> max;
	
// 	 can uncomment for a fixed length, comment other one
//	len = rand() % max + 1;
	
	for ( j = 0; j < iteration; j++ )
	{
		// this is the line to comment for fixed length
		len = rand() % max + 1;
		for ( i = 0; i < len; i++ )
		{
			temp = rand() % ( 127 - 33 ) + 33;
			str += temp;
		}
		
		cout << str << endl;
		str.clear();
	}
	
}

void genBool ( int iteration )
{
	bool obj;
	
	ull i;
	
	for ( i = 0; i < iteration; i++ )
	{
		obj = rand() % 2;
		cout << obj << " ";
	}

}

void genDouble ( int iteration )
{
	double num;
	ull numerator;
	ull denominator;
	ull i;
	
	for ( i = 0; i< iteration; i++ )
	{
		numerator = rand ();
		denominator = rand ();
		
		num = (double) numerator / denominator;
		
		cout << num << endl;
	}

}

int main ( )
{
	int choice;
	ull iterations;
	
	srand ( (unsigned) time(NULL) );
	
	do
	{
		cin >> choice;
		
		cin >> iterations;
		
		switch ( choice )
		{
			case 1:
				genInts ( iterations );
				break;
			case 2:
				genString ( iterations );
				break;
			case 3:
				genBool ( iterations );
				break;
			case 4:
				genDouble ( iterations );
                                break;
                        default:
                                break;
		
		}
	
	} while ( choice > 0 );
	
	
	return ( 0 );

}





