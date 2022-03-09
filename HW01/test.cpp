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

	char byte = 0b0;
	int counter = -1;

	bool good = true;

	/**
	 * @brief reads one byte from input, also sets error handling variables
	 * 
	 */
	void ReadByte()
	{
		if( !input.read( &byte, 1 ) )
			good = false;
		else
			good = input.good();
	}

public:

	bitClass( ifstream & input )
	:	input( input )
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

//

class branchClass
{
private:
	branchClass *right = nullptr;
	branchClass *left = nullptr;

	int realBytes = 0;
	uint32_t character = 0b0;

	/**
	 * @brief Converts input in ASCII character
	 * 
	 * @param inputBit bitClass for reading the bits from file
	 * @return true failure
	 * @return false success
	 */
	bool ConvertAscii( bitClass & inputBit )
	{
		bool bit;
		realBytes = 1;

		for( int i = 6; i >= 0; i-- ) {
			if( inputBit.GetBit( bit ) )
				return true;
			if( bit )
				character |= bit << i;
		}
		return false;
	}

	/**
	 * @brief Converts input in UTF-8 character
	 * 
	 * @param inputBit bitClass for reading the bits from file
	 * @return true failure
	 * @return false success
	 */
	bool ConvertUTF( bitClass & inputBit )
	{
		bool bit;
		uint32_t check = 0b0;
		int CheckShift = 0;

		int shift = -1;
		do {
			if( inputBit.GetBit( bit ) || ++realBytes > 4 )
				return true;
			shift += 8;
		} while( bit );

		if( realBytes == 1 )
			return true;
		else if( realBytes == 2 )
			CheckShift = 10;
		else if( realBytes == 3 )
			CheckShift = 15;
		else
			CheckShift = 20;

		// shifting first bits (1) as it is supposed to be in UTF-8
		//	-> 110 | 1110 | 11110
		for( int i = 0; i < realBytes; i++ )
			character |= 1 << shift--;
		shift--;
		
		// setting the character by shifting the input
		while( shift >= 0 ) {
			if( inputBit.GetBit( bit ) )
				return true;
			if( ( shift + 1 ) % 8 == 0 ) {
				if( !bit )
					return true;
				character |= bit << shift;
			}
			else if( ( shift + 2 ) % 8 == 0 ) {
				if( bit )
					return true;
			}
			else if( bit ) {
				character |= bit << shift;
				check |= bit << CheckShift--;
			}
			else
				CheckShift--;
			shift--;
		}

		// cout << check << " " << 0x10FFFF << endl;
		if( check > 0x10FFFF )
			return true;

		return false;
	}

public:

	branchClass()
		: right(nullptr),
		  left(nullptr)
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
			if( inputBit.GetBit( bit ) )
				return true;

			if( bit ) {
				if( ConvertUTF( inputBit ) )
					return true;
			}
			else {
				if( ConvertAscii( inputBit ) )
					return true;
			}

			return false;
		}
	
		right = new branchClass();
		left = new branchClass();

		if( !right || !left )
			return true;

		return left->Read( inputBit ) || right->Read( inputBit );
	}

	/**
	 * @brief Get the char of code name from input file
	 * 
	 * @param inputBit bitClass for reading each bits from the input
	 * @param ch saving the character
	 * @return true failure
	 * @return false success
	 */
	bool GetChar( bitClass & inputBit, uint32_t & ch, int & bytesCount )
	{
		if( realBytes != 0 ) {
			ch = character;
			bytesCount = realBytes;
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
			return right->GetChar( inputBit, ch, bytesCount );
		else
			return left->GetChar( inputBit, ch, bytesCount );
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

	/**
	 * @brief writes the specified number of bytes in ifstream byte by byte
	 * 
	 * @param ch 4 bytes maximum of data
	 * @param bytes number of bytes, to be sure in utf8
	 * @return true failure
	 * @return false success
	 */
	bool Write( uint32_t ch, int bytes )
	{
		for( int i = bytes - 1; i >= 0; i-- ) {
			char out = ( ch >> i * 8 ) & 0b11111111;

			if( !output.write( &out, 1 ) )
				return true;
		}
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
		uint32_t ch;
		int bytes;
		
		for( int i = 0; i < end; i++) {
			if( top.GetChar( inputBits, ch, bytes ) )
				return true;

			if( Write( ch, bytes ) )
				return true;
		}

		return false;
	}

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

		int end = 0b0;
		for( int i = 11; i >= 0; i -- ) {
			if( inputBits.GetBit( bit ) )
				return true;
			end |= bit << i;
		}

		if( end > 0 ) {
			if( ReadPartion( end ) )
				return true;
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
	// open, check and init instances
	ifstream input( inFileName, ios::binary | ios::in );
	ofstream output( outFileName, ios::binary | ios::out );
	if( !input.is_open() || !output.is_open() )
		return false;
	if( input.eof() || !input.good() || !output.good() )
		return false;

	branchClass top;
	bitClass inputBit( input );
	InOutClass reading( inputBit, output, top );

	// reads and saves accordingly whole tree of input
	// -> recursivelly calls itself, so it reaches the bottom lists
	if( top.Read( inputBit ) ) {
		top.Clear();
		return false;
	}

	// reading coded characters from input and directly writing encoded characters in output
	if( reading.Read() ) {
		reading.close();
		return false;
	}

	// clears allocated tree and closes streams
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

	assert(decompressFile("tests/special0.huf", "tempfile"));

	assert(!decompressFile("tests/special1.huf", "tempfile"));

	return 0;
}
#endif /* __PROGTEST__ */
