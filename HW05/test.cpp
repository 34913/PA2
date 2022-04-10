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

bool pairCompare( const pair<string, int> & a, const pair<string, int> & b )
{
	return a.second > b.second;
}

class CSupermarket
{
public:

	CSupermarket() {}

	void print()
	{
		cout << endl;
		auto dataIt = data.begin();
		while( dataIt != data.end() ) {
			cout << dataIt->first << endl;
			auto it = dataIt->second.begin();
			while( it != dataIt->second.end() ) {
				cout << "\t" << it->first.day << "." << it->first.month << "." << it->first.year << " " << it->second << endl;
				it++;
			}

			dataIt++;		
		}
		cout << endl;
	}

	CSupermarket & store( const string & name, const CDate & expiryDate, int count )
	{
		data[ name ][ expiryDate ] += count;

		return *this;
	}
	
	list<pair<string, int>> expired( const CDate & expDate ) const
	{
		multiset<pair<string, int>, bool(*)(const pair<string, int> &, const pair<string, int> &)> array( pairCompare );
		auto dataIt = data.begin();

		while( dataIt != data.end() ) {

			auto it = dataIt->second.begin();
			pair<string, int> item;

			if( it->first < expDate )
				item = { dataIt->first, it->second };
			else {
				dataIt++;
				continue;
			}
			it++;

			while( it != dataIt->second.end() ) {
				if( it->first < expDate )
					item.second += it->second;
				else
					break;

				it++;
			}
			array.insert( item );

			dataIt++;
		}

		list<pair<string, int>> records;
		auto arrIt = array.begin();
		while( arrIt != array.end() ) {
			records.push_back( (*arrIt) );
			arrIt++;
		}
		return records;
	}
	
	void sell( list<pair<string, int>> & array )
	{
		auto arrIt = array.begin();

		list<string> rm;

		while( arrIt != array.end() ) {
			if( arrIt->second == 0 ) {
				arrIt = array.erase( arrIt );
				continue;
			}
			
			auto dataIt = data.find( arrIt->first );

			if( dataIt == data.end() ) {
				if( !FindSimilar( arrIt->first, dataIt ) ) {
					++ arrIt;
					continue;
				}
			}

			if( SellExplicit( dataIt, arrIt, &rm ) )
				arrIt = array.erase( arrIt );
			else 
				++ arrIt;
		}

		auto it = rm.begin();
		while( it != rm.end() ) {
			data.erase( *it );
			++ it;
		}

		return;
	}

private:

	bool FindSimilarSimple( const string & name, unordered_map<string, map<CDate, int>>::iterator & dataIt )
	{
		dataIt = data.begin();
		unordered_map<string, map<CDate, int>>::iterator out;
		bool found = false;

		while( dataIt != data.end() ) {
			if( dataIt->first.length() != name.length() ) {
				++ dataIt;
				continue;
			}
			int diff = 0;
			
			for( int i = 0; i < name.length(); i++ ) {
				if( name[ i ] != dataIt->first[ i ] )
					if( ++ diff == 2 )
						break;
			}

			if( diff == 1 ) {
				if( found )
					return false;
				found = true;
				out = dataIt;
			}
		}

		dataIt = out;

		return found;
	}

	bool FindSimilar( const string & name, unordered_map<string, map<CDate, int>>::iterator & dataIt )
	{
		bool found = false;
		string copy = name;

		cout << name << endl;
		for( uint32_t i = 0; i < name.length(); i++ ) {
			if( CycleFind( copy, i, found, 'a', dataIt ) )
				return false;
			if( CycleFind( copy, i, found, 'A', dataIt ) )
				return false;


			copy[ i ] = name[ i ];
		}

		return found;
	}

	bool CycleFind( string & copy, int pos, bool & found, char offset, unordered_map<string, map<CDate, int>>::iterator & back )
	{
		char end = 'z' - 'a' + offset;
		cout << pos << " " << found << " " << (char)offset << endl;
		for( char diff = offset; diff <= end; diff++ ) {
			copy[ pos ] = diff;
			cout << copy << endl;

			auto it = data.find( copy );
			if( it != data.end() ) {
				if( found )
					return true;
				found = true;
				back = it;
			}
		}
		return false;
	}

	bool SellExplicit( unordered_map<string, map<CDate, int>>::iterator & dataIt, list<pair<string, int>>::iterator & arrIt, list<string> * rm )
	{
		if( dataIt->second.size() == 0 )
			return false;

		while( dataIt->second.size() != 0 ) {
			auto temp = dataIt->second.begin();

			if( arrIt->second > temp->second ) {
				arrIt->second -= temp->second;
				dataIt->second.erase( temp );
			}
			else {
				if ( arrIt->second == temp->second ) {
					dataIt->second.erase( temp );
					if( dataIt->second.size() == 0 )
						rm->push_back( dataIt->first );
				}
				else 
					temp->second -= arrIt->second;

				arrIt->second = 0;
				return true;
			}
		}

		rm->push_back( dataIt->first );

		return false;
	}
	
	unordered_map<string, map<CDate, int>> data;

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
	assert((l0 == list<pair<string, int>>{{"bread", 200}, {"beer", 50}, {"butter", 10}, {"okey", 5}}));

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
	assert((l5 == list<pair<string, int>>{{"bread", 93}, {"beer", 50}, {"okey", 5}}));

	s.store("Coke", CDate(2016, 12, 31), 10);

	list<pair<string, int>> l6{{"Cake", 1}, {"Coke", 1}, {"cake", 1}, {"coke", 1}, {"cuke", 1}, {"Cokes", 1}};
	s.sell(l6);
	assert(l6.size() == 3);
	assert((l6 == list<pair<string, int>>{{"cake", 1}, {"cuke", 1}, {"Cokes", 1}}));

	list<pair<string, int>> l7 = s.expired(CDate(2017, 1, 1));
	assert(l7.size() == 4);
	assert((l7 == list<pair<string, int>>{{"bread", 93}, {"beer", 50}, {"Coke", 7}, {"okey", 5}}));

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

	//

	s.store("bread", CDate(2016, 4, 30), 134)
		.store("bread", CDate(2016, 4, 30), 134)
		.store("bread", CDate(2016, 2, 30), 134)
		.store("butter", CDate(2015, 5, 11), 79)
		.store("butter", CDate(2015, 5, 10), 79)
		.store("butter", CDate(2012, 5, 10), 79)
		.store("beer", CDate(2018, 8, 10), 48)
		.store("beer", CDate(2017, 5, 10), 48)
		.store("beer", CDate(2017, 5, 8), 48)
		.store("bread", CDate(2016, 4, 25), 24)
		.store("okey", CDate(2016, 7, 18), 2);

	s.print();
	auto l16 = s.expired(CDate(2025, 1, 1));
	s.store("Butter", CDate(2020,1,1), 1);

	l16 = {{"ccccc", 10},{"butter",15},{"ccccd",100},{"Butter",222},{"butter",1},{"Butter",1} };
	s.sell(l16);
	assert(l16.size() == 3);
	assert((l16 == list<pair<string, int>>{{"ccccc", 10}, {"Butter",221}, {"Butter",1}}));

	CSupermarket s2;
	s2.store( "butter", CDate(2020,1,1), 1 );
	list<pair<string, int>> l17{{"Butter", 1}, {"butter",1}};
	s2.sell(l17);
	assert(l17.size() == 1);
	assert((l17 == list<pair<string, int>>{{"butter",1}}));

	s.print();

	list<pair<string, int>> l18{{"okay", 6}, {"okei",2},{"okay",1}};
	s.sell(l18);
	assert(l18.size() == 2);
	assert((l18 == list<pair<string, int>>{{"okei",1},{"okay",1}}));
	s.print();

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
