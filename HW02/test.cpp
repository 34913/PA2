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
	:	name( name ),
		addr( addr ),
		taxID( taxID )
	{}

	void AddSum( uint add ) { sumIncome += add; }

	uint SumIncome() { return sumIncome; }

};

//

class CVATRegister
{
private:
	vector<Company*> listById;
	vector<vector<Company*>> listByName;
	mutable vector<uint> listInvoice;

	mutable uint posID = 0;
	mutable uint posName = 0;
	mutable uint posAddr = 0;

	//

	bool FindID( const string & str) const
	{
		int from = 0;
		int to = listById.size() - 1;
		bool found = true;

		while( true ) {
			if( FindIDRec( str, from, to, found ) )
				return !found;
		}
	}

	bool FindIDRec( const string & str,
				 	int & from,
				 	int & to,
					bool & found ) const
	{
		if( to == from )
			posID = from;
		else
			posID = from + ( to - from ) / 2;
		
		int compare = str.compare( listById[ posID ]->taxID );
		
		if( compare == 0 )
			return true;
		else if( to == from ) {
			if( compare > 0 )
				posID++;
			found = false;
			
			return true;
		}
		if( compare > 0 )
			from = posID + 1;
		else
			to = posID - 1;
		
		if( to < from ) {
			found = false;
			
			return true;
		}

		return false;
	}

/*
	bool FindName( const string & str,
				   int from,
				   int to ) const
	{
		if( to - from == 0 )
			posName = from;
		else
			posName = from + ( to - from ) / 2;

		int compare = strcasecmp( str.c_str(), listByName[ posName ][ 0 ]->name.c_str() );
		if( compare == 0 )
			return false;
		else if( to - from == 0 ) {
			if( compare > 0 )
				posName++;

			return true;
		}

		if( compare > 0 )
			return FindName( str, posName, to );
		else
			return FindName( str, from, posName );
	}
*/

	bool FindName( const string & str) const
	{
		int from = 0;
		int to = listByName.size() - 1;
		bool found = true;

		while( true ) {
			if( FindNameRec( str, from, to, found ) )
				return !found;
		}		
	}


	bool FindNameRec( const string & str,
				 	  int & from,
				 	  int & to,
					  bool & found ) const
	{
		if( to == from )
			posName = from;
		else
			posName = from + ( to - from ) / 2;

		int compare = strcasecmp( str.c_str(), listByName[ posName ][ 0 ]->name.c_str() );

		if( compare == 0 )
			return true;
		else if( to == from ) {
			if( compare > 0 )
				posName++;
			found = false;

			return true;
		}

		if( compare > 0 )
			from = posName + 1;
		else
			to = posName - 1;

		if( to < from ) {
			found = false;

			return true;
		}
		
		return false;
	}

	bool FindAddr( const string & str ) const
	{
		int from = 0;
		int to = listByName[ posName ].size() - 1;
		bool found = true;

		while( true ) {
			if( FindAddrRec( str, from, to, found ) )
				return !found;
		}
	}

	bool FindAddrRec( const string & str,
				 	  int & from,
				 	  int & to,
					  bool & found ) const
	{
		if( to == from )
			posAddr = from;
		else
			posAddr = from + ( to - from ) / 2;

		int compare = strcasecmp( str.c_str(), listByName[ posName ][ posAddr ]->addr.c_str() );

		if( compare == 0 )
			return true;
		else if( to == from ) {
			if( compare > 0 )
				posAddr++;
			found = false;

			return true;
		}
		if( compare > 0 )
			from = posAddr + 1;
		else
			to = posAddr - 1;

		if( to < from ) {
			found = false;

			return true;
		}

		return false;
	}

	void InvoiceAmount( Company *ptr, 
				  		uint amount )
	{
		ptr->AddSum( amount );

		listInvoice.push_back( amount );
	}

public:

	void PrintOut( void )
	{
		cout << "----------------------" << endl;

		for( uint i = 0; i < (uint)listById.size(); i++ )
			cout << listById[ i ]->name << " - " << listById[ i ]->addr << "\n\t" << listById[ i ]->taxID << endl;

		cout << endl;

		for( uint i = 0; i < (uint)listByName.size(); i++ ) {
			cout << listByName[ i ][ 0 ]->name << endl;

			for( uint y = 0; y < (uint)listByName[ i ].size(); y++ )
				cout << "\t - " << listByName[ i ][ y ]->addr << " - id: " << listByName[ i ][ y ]->taxID << endl;
		}

		cout << endl;
	}

	CVATRegister( void )
	{}
	~CVATRegister( void )
	{
		for( uint i = 0; i < (uint)listById.size(); i++ )
			delete listById[ i ];
	}
	
	bool newCompany( const string &name,
					 const string &addr,
					 const string &taxID )
	{
		Company *created;
	
		if( listById.size() == 0 ) {
			created = new Company( name, addr, taxID );

			listById.push_back( created );
			
			listByName.push_back( vector<Company*>() );
			listByName[ 0 ].push_back( created );

			return true;
		}

		if( !FindID( taxID ) )
			return false;
		if( !FindName( name ) ) {
			if( !FindAddr( addr ) )
				return false;

			created = new Company( name, addr, taxID );

			listByName[ posName ].insert( listByName[ posName ].begin() + posAddr, created );
		}
		else {
			created = new Company( name, addr, taxID );

			listByName.insert( listByName.begin() + posName, vector<Company*>() );
			listByName[ posName ].push_back( created );
		}

		listById.insert( listById.begin() + posID, created );

		return true;
	}
	
	//

	bool cancelCompany( const string & name,
					    const string & addr )
	{
		if( listById.size() == 0 )
			return false;
		if( FindName( name ) )
			return false;
		if ( FindAddr( addr ) )
			return false;

		Company *toErase = listByName[ posName ][ posAddr ];
		listByName[ posName ].erase( listByName[ posName ].begin() + posAddr );
		if( listByName[ posName ].size() == 0 )
			listByName.erase( listByName.begin() + posName );

		FindID( toErase->taxID );
		listById.erase( listById.begin() + posID );

		delete toErase;

		return true;
	}
	
	bool cancelCompany( const string & taxID )
	{		
		if( listById.size() == 0 )
			return false;
		
		if( FindID( taxID ) )
			return false;

		Company *toErase = listById[ posID ];
		listById.erase( listById.begin() + posID );

		FindName( toErase->name );
		FindAddr( toErase->addr );

		listByName[ posName ].erase( listByName[ posName ].begin() + posAddr );
		if( listByName[ posName ].size() == 0 )
			listByName.erase( listByName.begin() + posName );

		delete toErase;

		return true;
	}

	//

	bool invoice( const string &taxID,
				  unsigned int amount )
	{		
		if( listById.size() == 0 )
			return false;
		if( FindID( taxID ) )
			return false;

		InvoiceAmount( listById[ posID ], amount );

		return true;
	}

	bool invoice( const string &name,
				  const string &addr,
				  unsigned int amount )
	{
		if( listById.size() == 0 )
			return false;
		if( FindName( name ) )
			return false;
		if( FindAddr( addr ) )
			return false;

		InvoiceAmount( listByName[ posName ][ posAddr ], amount );
		
		return true;
	}
	
	//

	bool audit( const string &name,
			    const string &addr,
			    unsigned int &sumIncome ) const
	{
		if( listById.size() == 0 )
			return false;
		if( FindName( name ) )
			return false;
		if( FindAddr( addr ) )
			return false;

		sumIncome = listByName[ posName ][ posAddr ]->SumIncome();

		return true;
	}
	bool audit( const string &taxID,
			    unsigned int &sumIncome ) const
	{
		if( listById.size() == 0 )
			return false;
		if( FindID( taxID ) )
			return false;

		sumIncome = listById[ posID ]->SumIncome();

		return true;
	}
	
	//

	bool firstCompany( string &name,
					   string &addr ) const
	{
		if( listByName.size() == 0 )
			return false;

		name = listByName[ 0 ][ 0 ]->name;
		addr = listByName[ 0 ][ 0 ]->addr;

		return true;
	}
	bool nextCompany( string &name,
					  string &addr ) const
	{
		if( listById.size() == 0 )
			return false;
		if( FindName( name ) )
			return false;
		if( FindAddr( addr ) )
			return false;

		if( ++posAddr == listByName[ posName ].size() ) {
			if( ++posName == listByName.size() )
				return false;
			posAddr = 0;
		}
		
		Company* ptr = listByName[ posName ][ posAddr ];

		name = ptr->name;
		addr = ptr->addr;

		return true;
	}

	//

	unsigned int medianInvoice(void) const
	{
		if( listInvoice.size() == 0 )
			return 0;
		
		sort( listInvoice.begin(), listInvoice.end() );

		uint middle = listInvoice.size() / 2;
		uint median = listInvoice[ middle ];

		if( listInvoice.size() % 2 == 0 )
			median = max( median, listInvoice[ middle - 1 ] );			

		return median;
	}

};

//

#ifndef __PROGTEST__
int main( void )
{
	string name, addr;
	unsigned int sumIncome;

	CVATRegister b1;
	assert(b1.newCompany("ACME", "Thakurova", "666/666"));
	assert(b1.newCompany("ACME", "Kolejni", "666/666/666"));
	assert(b1.newCompany("Dummy", "Thakurova", "123456"));
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

	CVATRegister b2;
	
	assert(b2.newCompany("ACME", "Kolejni", "abcdef"));
	assert(b2.newCompany("Dummy", "Kolejni", "123456"));
	assert(!b2.newCompany("AcMe", "kOlEjNi", "1234"));
	assert(b2.newCompany("Dummy", "Thakurova", "ABCDEF"));
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

	CVATRegister b3;
	assert(!b3.cancelCompany("asd"));
	assert(!b3.cancelCompany("asd", "asd"));
	assert(!b3.invoice("asd", 1000));

	assert(b3.newCompany("aa", "a", "123"));
	assert(b3.newCompany("a", "aa", "456"));

	assert(b3.cancelCompany("aa","a"));
	assert(b3.cancelCompany("456"));
	
	//

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
