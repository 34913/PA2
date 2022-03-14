#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

//


string StrToLower( const string & str )
{
	string output = "";
	for( ulong i = 0; i < str.length(); i++ ) {
		char ch = str[ i ];
		if( isupper( ch ) )
			output += tolower( ch );
		else
			output += ch;
	}
	return output;
}

//

class Company
{
private:
	unsigned int sumIncome = 0;

public:

	const string name;
	const string addr;
	const string taxID;

	Company( const string &name,
			 const string &addr,
			 const string &taxID )
	:	name( StrToLower( name ) ),
		addr( StrToLower( addr ) ),
		taxID( taxID )
	{}

	void AddSum( uint add ) { sumIncome += add; }

	uint SumIncome() { return sumIncome; }

	Company operator=( Company x )
	{ 
		cout << "why" << endl;
		return x;
	}

};

//

class CVATRegister
{

private:
	vector<Company> listById;
	vector<vector<Company>> listByName;
	vector<uint> faktury;

	const string & CompareFindByID( uint *pos, uint temp )
	{
		*pos = temp;
		return listById[ temp ].taxID;
	}
	
	const string & CompareFindByAddr( uint *pos, uint temp )
	{
		pos[ 1 ] = temp;
		return listByName[ pos[ 0 ] ][ temp ].addr;
	}

	const string & CompareFindByName( uint *pos, uint temp )
	{
		pos[ 0 ] = temp;
		return listByName[ temp ][ 0 ].name;
	}

	//

	bool Find( const string & str,
			   int from,
			   int to,
			   const string &(CVATRegister::*compareFunc)( uint *, uint ),
			   uint *pos )
	{
		if( to - from < 0 ) {
			return false;
		}

		uint temp;
		if( to - from == 1 )
			temp = from;
		else
			temp = from + ( to - from ) / 2;

		int compare = str.compare( (this->*compareFunc)( pos, temp ) );
		if( compare == 0 ) {
			return true;
		}
		else if( to - from == 0 ) {
			if( compare > 0 ) {
				if( compareFunc == &CVATRegister::CompareFindByAddr )
					pos[ 1 ] += 1;
				else
					*pos += 1;
			}

			return false;
		}

		if( compare > 0 )
			return Find( str, *pos + 1, to, compareFunc, pos );
		else
			return Find( str, from, *pos - 1, compareFunc, pos );

	}

public:
	CVATRegister(void)
	{}
	~CVATRegister(void)
	{}
	
	bool newCompany(const string &name,
					const string &addr,
					const string &taxID)
	{
		Company created( name, addr, taxID );

		uint posN[2], posID;
		if( listById.size() == 0 ) {
			listById.insert( listById.begin(), created );
			
			listByName.insert( listByName.begin(), vector<Company>() );
			listByName[ 0 ].insert( listByName[ 0 ].begin(), created );

			return true;
		}

		if( Find( created.taxID, 0, listById.size() - 1, &CVATRegister::CompareFindByID, &posID ) )
			return false;

		if( Find( created.name, 0, listByName.size() - 1, &CVATRegister::CompareFindByName, posN ) ) {
			if( Find( created.addr, 0, listByName[ posN[ 0 ] ].size() - 1, &CVATRegister::CompareFindByAddr, posN ) )
				return false;
		}
		else {
			listById.insert( listById.begin() + posID, created );
			listByName.insert( listByName.begin() + posN[ 0 ], vector<Company>() );
			listByName[ posN[ 0 ] ].insert( listByName[ posN[ 0 ] ].begin(), created );
			return true;
		}

		listById.insert( listById.begin() + posID, created );
		listByName[ posN[ 0 ] ].insert( listByName[ posN[ 0 ] ].begin() + posN[ 1 ], created );

		return true;
	}

	void PrintOut()
	{
		cout << endl;

		for( int i = 0; i < (int)listById.size(); i++ )
			cout << listById[ i ].taxID << endl;
		
		cout << endl;

		for( int i = 0; i < (int)listByName.size(); i++ ) {
			cout << listByName[ i ][ 0 ].name << endl;
			for( int y = 0; y < (int)listByName[ i ].size(); y++ )
				cout << "\t" << listByName[ i ][ y ].addr << endl;
		}

		cout << endl;
	}
	
	//
	
	/*

	bool cancelCompany(const string &name,
					   const string &addr);
	bool cancelCompany(const string &taxID);
	
	//

	bool invoice(const string &taxID,
				 unsigned int amount);
	bool invoice(const string &name,
				 const string &addr,
				 unsigned int amount);

	//

	bool audit(const string &name,
			   const string &addr,
			   unsigned int &sumIncome) const;
	bool audit(const string &taxID,
			   unsigned int &sumIncome) const;
	
	//

	bool firstCompany(string &name,
					  string &addr) const;
	bool nextCompany(string &name,
					 string &addr) const;

	//

	unsigned int medianInvoice(void) const;

	*/

};

#ifndef __PROGTEST__
int main(void)
{
	string name, addr;
	unsigned int sumIncome;

	CVATRegister b1;
	assert(b1.newCompany("ACME", "Thakurova", "666/666"));
	assert(b1.newCompany("ACME", "Kolejni", "666/666/666"));
	assert(b1.newCompany("Dummy", "Thakurova", "123456"));
	
	b1.PrintOut();

	/*
	assert(b1.invoice("666/666", 2000));
	assert(b1.medianInvoice() == 2000);
	assert(b1.invoice("666/666/666", 3000));
	assert(b1.medianInvoice() == 3000);
	assert(b1.invoice("123456", 4000));
	assert(b1.medianInvoice() == 3000);
	assert(b1.invoice("aCmE", "Kolejni", 5000));
	assert(b1.medianInvoice() == 4000);
	assert(b1.audit("ACME", "Kolejni", sumIncome) && sumIncome == 8000);
	assert(b1.audit("123456", sumIncome) && sumIncome == 4000);
	assert(b1.firstCompany(name, addr) && name == "ACME" && addr == "Kolejni");
	assert(b1.nextCompany(name, addr) && name == "ACME" && addr == "Thakurova");
	assert(b1.nextCompany(name, addr) && name == "Dummy" && addr == "Thakurova");
	assert(!b1.nextCompany(name, addr));
	assert(b1.cancelCompany("ACME", "KoLeJnI"));
	assert(b1.medianInvoice() == 4000);
	assert(b1.cancelCompany("666/666"));
	assert(b1.medianInvoice() == 4000);
	assert(b1.invoice("123456", 100));
	assert(b1.medianInvoice() == 3000);
	assert(b1.invoice("123456", 300));
	assert(b1.medianInvoice() == 3000);
	assert(b1.invoice("123456", 200));
	assert(b1.medianInvoice() == 2000);
	assert(b1.invoice("123456", 230));
	assert(b1.medianInvoice() == 2000);
	assert(b1.invoice("123456", 830));
	assert(b1.medianInvoice() == 830);
	assert(b1.invoice("123456", 1830));
	assert(b1.medianInvoice() == 1830);
	assert(b1.invoice("123456", 2830));
	assert(b1.medianInvoice() == 1830);
	assert(b1.invoice("123456", 2830));
	assert(b1.medianInvoice() == 2000);
	assert(b1.invoice("123456", 3200));
	assert(b1.medianInvoice() == 2000);
	assert(b1.firstCompany(name, addr) && name == "Dummy" && addr == "Thakurova");
	assert(!b1.nextCompany(name, addr));
	assert(b1.cancelCompany("123456"));
	assert(!b1.firstCompany(name, addr));
*/

	CVATRegister b2;
		
	assert(b2.newCompany("ACME", "Kolejni", "abcdef"));
	assert(b2.newCompany("Dummy", "Kolejni", "123456"));
	assert(!b2.newCompany("AcMe", "kOlEjNi", "1234"));
	assert(b2.newCompany("Dummy", "Thakurova", "ABCDEF"));
	b2.newCompany("ABD", "lol", "ABCDEFG");
	b2.newCompany("abd", "pol5", "ABCDEFE");
	b2.newCompany("abd", "pol5asd", "ABCDEf");

	b2.PrintOut();

	/*
	assert(b2.medianInvoice() == 0);
	assert(b2.invoice("ABCDEF", 1000));
	assert(b2.medianInvoice() == 1000);
	assert(b2.invoice("abcdef", 2000));
	assert(b2.medianInvoice() == 2000);
	assert(b2.invoice("aCMe", "kOlEjNi", 3000));
	assert(b2.medianInvoice() == 2000);
	assert(!b2.invoice("1234567", 100));
	assert(!b2.invoice("ACE", "Kolejni", 100));
	assert(!b2.invoice("ACME", "Thakurova", 100));
	assert(!b2.audit("1234567", sumIncome));
	assert(!b2.audit("ACE", "Kolejni", sumIncome));
	assert(!b2.audit("ACME", "Thakurova", sumIncome));
	assert(!b2.cancelCompany("1234567"));
	assert(!b2.cancelCompany("ACE", "Kolejni"));
	assert(!b2.cancelCompany("ACME", "Thakurova"));
	assert(b2.cancelCompany("abcdef"));
	assert(b2.medianInvoice() == 2000);
	assert(!b2.cancelCompany("abcdef"));
	assert(b2.newCompany("ACME", "Kolejni", "abcdef"));
	assert(b2.cancelCompany("ACME", "Kolejni"));
	assert(!b2.cancelCompany("ACME", "Kolejni"));

	*/
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
