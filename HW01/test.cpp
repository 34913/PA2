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

#define CHUNK_SIZE 4096

//

class bitClass 
{
protected:
	ifstream & input;

private:
	const int maxByte = 8;

	char byte;
	int counter;

	bool good = true;

	/**
	 * @brief reads one byte from input, also sets error handling variables
	 * 
	 */
	void ReadByte()
	{
		if( !input.read( &byte, 1 ) )
			good = false;
		good = input.good();
	}

public:

	bitClass( ifstream & input )
	:	input( input ),
		counter( -1 )
	{}

	/**
	 * @brief Get the bit from input
	 * 
	 * @param bit bit to be set from input
	 * @return true failure
	 * @return false success
	 */
	bool GetBit( bool & bit )
	{
		if( counter == -1 ) {
			ReadByte();
			if( !good )
				return true;
			counter = maxByte - 1;
		}

		bit = byte & ( 1 << counter-- );
		
		return false;
	}

	bool isGood() { return good; }
	
	void close() { input.close(); }
};

class branchClass
{
private:
	branchClass *right;
	branchClass *left;

	uint32_t character;

	/**
	 * @brief converts input in UTF-8 character 
	 * (cant save it, just yet, ups)
	 * 
	 * @param inputBit bitClass for reading the bits from file
	 * @return true failure
	 * @return false success
	 */
	bool Convert( bitClass & inputBit )
	{
		bool bit;
		int shift = 6;
		int add = 4;
		bool first = true;

		for( int y = 0; y < 4; y++ ) {
			if( inputBit.GetBit( bit ) )
				return true;
			cout << bit;
			int count = 0;
			while( bit ) {
				if( inputBit.GetBit( bit ) )
					return true;
				cout << bit;

				count++;
				
				if( first && count > 1 )
					shift += add++;
			}
			if( !first && count != 1 )
				return true;
			else if( first )
				cout << endl << shift << endl;

			for( int i = 6 - count; i >= 0; i -- ) {
				if( inputBit.GetBit( bit ) )
					return true;
				character |= bit << shift--;
			}

			first = false;

			if( shift == -1 )
				break;
		}

		return false;
	}

public:

	branchClass()
		: right(nullptr),
		  left(nullptr),
		  character(0x0)
	{}

	/**
	 * @brief reads and creates all the branches for storing the tree of code names 
	 * for each character on input of hufman tree
	 * 
	 * @param inputBit bitClass for reading the bits from file
	 * @return true failure
	 * @return false success
	 */
	bool Read( bitClass & inputBit )
	{
		bool bit;
		if( inputBit.GetBit( bit ) )
			return true;

		if( bit ) {
			character = 0x0;
			if( Convert( inputBit) )
				return true;
		}
		else {
			right = new branchClass();
			left = new branchClass();

			if( !right || !left )
				return true;

			return left->Read( inputBit ) || right->Read( inputBit );
		}
		return false;
	}

	/**
	 * @brief Get the char of code name from input file
	 * 
	 * @param inputBit bitClass for reading each bits from the input
	 * @param ch saving the character
	 * @return true failure
	 * @return false success
	 */
	bool GetChar( bitClass & inputBit, char & ch )
	{
		if( character != 0x0 ) {
			ch = character;
			return false;
		}
		// dont need to check left, both has to be nullptr
		// if the list is on the end
		else if( right == nullptr )
			return true;

		bool bit;
		if( inputBit.GetBit( bit ) )
			return true;


		if( bit )
			return right->GetChar( inputBit, ch );
		else
			return left->GetChar( inputBit, ch );
	}

	/**
	 * @brief clearing all subtrees of this specified branch
	 * 
	 */
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

class InOutClass
{
protected:
	bitClass & inputBits;
	ofstream & output;
	branchClass & top;

public:

	InOutClass( bitClass & inputBits, ofstream & output, branchClass & top )
	:	inputBits( inputBits ),
		output( output ),
		top(top)
	{}

	/**
	 * @brief reads the rest of the input file
	 * converts it to the characters in output file in the process
	 * 
	 * @return true 
	 * @return false 
	 */
	bool Read()
	{
		bool bit;
		while( true ) {
			if( inputBits.GetBit( bit ) )
				return true;
			
			if( !bit )
				break;
	
			if( ReadPartion( CHUNK_SIZE ) )
				return true;
		}

		int end = 0x0;
		for( int i = 11; i >= 0; i -- ) {
			if( inputBits.GetBit( bit ) )
				return true;
			end |= bit << i;
		}

		if( ReadPartion( end ) )
			return true;

		return false;
	}

	/**
	 * @brief reads just partion of input
	 * 
	 * @param end specifies how many bits has to be read
	 * @return true failure
	 * @return false success
	 */
	bool ReadPartion( int end )
	{
		char ch;
		int charNum = 0;

		while( charNum < end ) {
			ch = 0x0;

			if( top.GetChar( inputBits, ch ) )
				return true;

			if( !output.write( &ch, 1) )
				return true;

			charNum ++;
		}

		return false;
	}

	void close()
	{
		inputBits.close();
		output.close();
		top.Clear();
	}

};

//

bool decompressFile(const char *inFileName, const char *outFileName)
{
	ifstream input( inFileName, ios::binary | ios::in );
	ofstream output( outFileName, ios::binary | ios::out );
	if( !input.is_open() || !output.is_open() )
		return false;
	if( input.eof() || !input.good() || !output.good() )
		return false;

	branchClass top;
	bitClass inputBit( input );
	
	if( top.Read( inputBit ) )
		return false;

	InOutClass reading( inputBit, output, top );
	if( reading.Read() ) {
		reading.close();
		return false;
	}

	reading.close();
	
	return true;
}

bool compressFile(const char *inFileName, const char *outFileName)
{
	// keep this dummy implementation (no bonus) or implement the compression (bonus)
	return false;
}

#ifndef __PROGTEST__
bool identicalFiles(const char *fileName1, const char *fileName2)
{
	string in = "cmp -s " + string(fileName1) + " " + string(fileName2);
	return !system( in.c_str() );
}

int main(void)
{

/*
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

*/
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
