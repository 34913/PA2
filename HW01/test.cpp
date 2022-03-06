#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <memory>
#include <functional>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

#include <math.h>

//

bool GetBit( ifstream & input ) 
{
	static char byte;
	static int counter = 0;

	if( counter == 0 )
		input.read( &byte, 1 );
	
	bool r = byte & ( 1 << abs(counter - 7) );

	if( ++counter == 8 )
		counter = 0;

	return r;
}

//

class branch
{

private:
	branch *Create()
	{
		return new branch();
	}

	branch *right;
	branch *left;

public:
	char character = 0;

	branch()
		: right(nullptr),
		  left(nullptr)
	{
	}

	branch *GetRight()
	{
		return right;
	}
	branch *GetLeft()
	{
		return left;
	}

	//

	bool Read( ifstream & input, char & buffer, int lastPos )
	{
		
	}

	//

	void Clear()
	{
		if( right != nullptr ) {
			right->Clear();
			delete right;
		}
		if( left != nullptr ) {
			left->Clear();
			delete left;
		}		
	}

};

//

//

bool decompressFile(const char *inFileName, const char *outFileName)
{
	ifstream input( inFileName, ios::binary | ios::in );
	if( !input.is_open() )
		return false;
	if( input.eof() )
		return false;

	branch top;

	input.seekg( 0, ios::end );
	streampos size = input.tellg();
	input.seekg( 0, ios::beg );

	char *block = new char[size];
    input.read( block, size );

	for( int i = 0; i < size; i ++ ) {
		for( int y = 7; y >= 0; y-- ) {
			bool bit = (block[ i ] & (1 << y));
			cout << bit << " ";
		}
		cout << " - " << (int)block[ i ] << endl;
	}
	

/*
	if( top.Read( input ) ) 
		return false;

*/
	

	input.close();


	return false;
}

bool compressFile(const char *inFileName, const char *outFileName)
{
	// keep this dummy implementation (no bonus) or implement the compression (bonus)
	return false;
}
#ifndef __PROGTEST__
bool identicalFiles(const char *fileName1, const char *fileName2)
{
	// todo
	return false;
}

int main(void)
{
	assert(decompressFile("tests/test0.huf", "tempfile"));
	assert(identicalFiles("tests/test0.orig", "tempfile"));

	assert(decompressFile("tests/test1.huf", "tempfile"));
	assert(identicalFiles("tests/test1.orig", "tempfile"));

	assert(decompressFile("tests/test2.huf", "tempfile"));
	assert(identicalFiles("tests/test2.orig", "tempfile"));

	assert(decompressFile("tests/test3.huf", "tempfile"));
	assert(identicalFiles("tests/test3.orig", "tempfile"));

	assert(decompressFile("tests/test4.huf", "tempfile"));
	assert(identicalFiles("tests/test4.orig", "tempfile"));

	assert(!decompressFile("tests/test5.huf", "tempfile"));

	assert(decompressFile("tests/extra0.huf", "tempfile"));
	assert(identicalFiles("tests/extra0.orig", "tempfile"));

	assert(decompressFile("tests/extra1.huf", "tempfile"));
	assert(identicalFiles("tests/extra1.orig", "tempfile"));

	assert(decompressFile("tests/extra2.huf", "tempfile"));
	assert(identicalFiles("tests/extra2.orig", "tempfile"));

	assert(decompressFile("tests/extra3.huf", "tempfile"));
	assert(identicalFiles("tests/extra3.orig", "tempfile"));

	assert(decompressFile("tests/extra4.huf", "tempfile"));
	assert(identicalFiles("tests/extra4.orig", "tempfile"));

	assert(decompressFile("tests/extra5.huf", "tempfile"));
	assert(identicalFiles("tests/extra5.orig", "tempfile"));

	assert(decompressFile("tests/extra6.huf", "tempfile"));
	assert(identicalFiles("tests/extra6.orig", "tempfile"));

	assert(decompressFile("tests/extra7.huf", "tempfile"));
	assert(identicalFiles("tests/extra7.orig", "tempfile"));

	assert(decompressFile("tests/extra8.huf", "tempfile"));
	assert(identicalFiles("tests/extra8.orig", "tempfile"));

	assert(decompressFile("tests/extra9.huf", "tempfile"));
	assert(identicalFiles("tests/extra9.orig", "tempfile"));

	return 0;
}
#endif /* __PROGTEST__ */
