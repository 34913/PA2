#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CDate
{
public:
	// todo

	int year;
	int month;
	int day;

	CDate( int year, int month, int day )
	{
		this->year = year;
		this->month = month;
		this->day = day;
	}

	bool operator < ( const CDate & x ) const
	{
		if( year < x.year )
			return true;
		else if( year != x.year )
			return false;
		
		if( month < x.month )
			return true;
		else if( month != x.month )
			return false;
		
		if( day < x.day )
			return true;
		return false;
	}
};

bool cmp( const CDate & a, const CDate & b )
{
	return a < b;
}

class CSupermarket
{
public:
	
	CSupermarket()
	{
		records = new list<pair<string, int>>();
	}

	CSupermarket & store( string name, CDate expiryDate, int count )
	{
		data[ name ][ expiryDate ] += count;

		return *this;
	}
	
	list<pair<string, int>> & expired( CDate expDate )
	{		
		records->clear();
		auto dataIt = data.begin();

		while( dataIt != data.end() ) {

			auto it = dataIt->second.begin();

			if( it->first < expDate )
				records->push_back( { dataIt->first, it->second } );
			it++;

			while( it != dataIt->second.end() ) {
				if( it->first < expDate )
					records->back().second += it->second;
				else
					break;

				it++;
			}

			dataIt++;
		}
		return *records;
	}
	
	void sell( list<pair<string, int>> & array )
	{
		// maybe save all iterators in some list and then pop them at once

		auto it = array.begin();
		
		while( it != array.end() ) {
			auto mapIt = data.find( it->first );

			if( mapIt == data.end() ) {
				if( SadButTrue( it->first, mapIt ) ) {
					it++;
					continue;
				}
			}

			if( SeekAndDestroy( it, mapIt ) )
				it = array.erase( it );

			if( mapIt->second.size() == 0 )
				data.erase( mapIt->first );
		}
	}

	~CSupermarket()
	{
		delete records;
	}

private:

	bool SadButTrue( string & original, unordered_map<string, map<CDate, int>>::iterator & mapIt )
	{
		string copy = original;
		bool found = false;

		for( uint32_t i = 0; i < copy.length(); i++ ) {
			if( NothingElseMatters( 'a', copy, i, mapIt, found )
				|| NothingElseMatters( 'A', copy, i, mapIt, found ) )
				return true;

			copy[ i ] = original[ i ];
		}

		if( found )
			return false;
		return true;
	}

	bool NothingElseMatters( int offset, string & copy, int pos, unordered_map<string, map<CDate, int>>::iterator & mapIt, bool & found )
	{
		for( int diff = offset; diff < 'z' - 'a' + offset; diff++ ) {
			copy[ pos ] = diff;
			auto used = data.find( copy );

			if( used != data.end() ) {
				if( found )
					return true;
				found = true;
				mapIt = used;
			}
		}

		return false;
	}

	bool SeekAndDestroy( list<pair<string, int>>::iterator & it, unordered_map<string, map<CDate, int>>::iterator mapIt )
	{
		while( true ) {
			if( it->second > mapIt->second.begin()->second ) {
				it->second -= mapIt->second.begin()->second;
				mapIt->second.erase( mapIt->second.begin() );
			}
			else {
				if( it->second == mapIt->second.begin()->second )
					mapIt->second.erase( mapIt->second.begin() );
				else
					mapIt->second.begin()->second -= it->second;
				
				return true;
			}

			if( mapIt->second.size() == 0 ) {
				it ++;
				return false;
			}
		}
	}

	unordered_map<string, map<CDate, int>> data;

	list<pair<string, int>> *records;

};

#ifndef __PROGTEST__
int main(void)
{
	CSupermarket s;

	s.store("bread", CDate(2016, 4, 30), 100)
		.store("butter", CDate(2016, 5, 10), 10)
		.store("beer", CDate(2016, 8, 10), 50)
		.store("bread", CDate(2016, 4, 25), 100)
		.store("okey", CDate(2016, 7, 18), 5);

	list<pair<string, int>> l0 = s.expired(CDate(2018, 4, 30));
	assert(l0.size() == 4);
	assert((l0 == list<pair<string, int>>{ {"okey", 5}, {"beer", 50}, {"butter", 10}, {"bread", 200} } ));

	list<pair<string, int>> l1{{"bread", 2}, {"Coke", 5}, {"butter", 20}};
	s.sell(l1);
	assert(l1.size() == 2);
	assert((l1 == list<pair<string, int>>{{"Coke", 5}, {"butter", 10}}));

	list<pair<string, int>> l2 = s.expired(CDate(2016, 4, 30));
	assert(l2.size() == 1);
	assert((l2 == list<pair<string, int>>{{"bread", 98}}));

	list<pair<string, int>> l3 = s.expired(CDate(2016, 5, 20));
	assert(l3.size() == 1);
	assert((l3 == list<pair<string, int>>{{"bread", 198}}));

	list<pair<string, int>> l4{{"bread", 105}};
	s.sell(l4);
	assert(l4.size() == 0);
	assert((l4 == list<pair<string, int>>{}));

	list<pair<string, int>> l5 = s.expired(CDate(2017, 1, 1));
	assert(l5.size() == 3);
	assert((l5 == list<pair<string, int>>{{"okey", 5}, {"beer", 50}, {"bread", 93}}));

	s.store("Coke", CDate(2016, 12, 31), 10);

	cout << l5.size() << endl;
	for( auto x : l5 )
		cout << x.first << " " << x.second << endl;

	list<pair<string, int>> l6{{"Cake", 1}, {"Coke", 1}, {"cake", 1}, {"coke", 1}, {"cuke", 1}, {"Cokes", 1}};
	s.sell(l6);
	assert(l6.size() == 3);
	assert((l6 == list<pair<string, int>>{{"cake", 1}, {"cuke", 1}, {"Cokes", 1}}));

	cout << l6.size() << endl;
	for( auto x : l6 )
		cout << x.first << " " << x.second << endl;

	list<pair<string, int>> l7 = s.expired(CDate(2017, 1, 1));
	assert(l7.size() == 4);
	assert((l7 == list<pair<string, int>>{{"bread", 93}, {"beer", 50}, {"Coke", 7}, {"okey", 5}}));

	cout << l7.size() << endl;
	for( auto x : l7 )
		cout << x.first << " " << x.second << endl;

	s.store("cake", CDate(2016, 11, 1), 5);

	list<pair<string, int>> l8{{"Cake", 1}, {"Coke", 1}, {"cake", 1}, {"coke", 1}, {"cuke", 1}};
	s.sell(l8);
	assert(l8.size() == 2);
	assert((l8 == list<pair<string, int>>{{"Cake", 1}, {"coke", 1}}));

	list<pair<string, int>> l9 = s.expired(CDate(2017, 1, 1));
	assert(l9.size() == 5);
	assert((l9 == list<pair<string, int>>{{"bread", 93}, {"beer", 50}, {"Coke", 6}, {"okey", 5}, {"cake", 3}}));

	list<pair<string, int>> l10{{"cake", 15}, {"Cake", 2}};
	s.sell(l10);
	assert(l10.size() == 2);
	assert((l10 == list<pair<string, int>>{{"cake", 12}, {"Cake", 2}}));

	list<pair<string, int>> l11 = s.expired(CDate(2017, 1, 1));
	assert(l11.size() == 4);
	assert((l11 == list<pair<string, int>>{{"bread", 93}, {"beer", 50}, {"Coke", 6}, {"okey", 5}}));

	list<pair<string, int>> l12{{"Cake", 4}};
	s.sell(l12);
	assert(l12.size() == 0);
	assert((l12 == list<pair<string, int>>{}));

	list<pair<string, int>> l13 = s.expired(CDate(2017, 1, 1));
	assert(l13.size() == 4);
	assert((l13 == list<pair<string, int>>{{"bread", 93}, {"beer", 50}, {"okey", 5}, {"Coke", 2}}));

	list<pair<string, int>> l14{{"Beer", 20}, {"Coke", 1}, {"bear", 25}, {"beer", 10}};
	s.sell(l14);
	assert(l14.size() == 1);
	assert((l14 == list<pair<string, int>>{{"beer", 5}}));

	s.store("ccccb", CDate(2019, 3, 11), 100)
		.store("ccccd", CDate(2019, 6, 9), 100)
		.store("dcccc", CDate(2019, 2, 14), 100);

	list<pair<string, int>> l15{{"ccccc", 10}};
	s.sell(l15);
	assert(l15.size() == 1);
	assert((l15 == list<pair<string, int>>{{"ccccc", 10}}));

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
