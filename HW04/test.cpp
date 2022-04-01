#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>

using namespace std;
#endif /* __PROGTEST__ */

#define PLUS 100

class record
{
private:

	uint8_t *arr = nullptr;

	uint32_t size = 0;
	uint32_t allocated = 0;
	uint32_t pos = 0;

	uint32_t occurs = 1;

	/**
	 * @brief Construct a new record object and copy all data
	 * used interanally to make copy on edit (saving memory)
	 * 
	 * @param r record to be based on
	 */
	record( record *r )
	{
		size = r->size;
		allocated = r->allocated;
		pos = r->pos;

		arr = new uint8_t[ allocated ];

		for( uint32_t i = 0; i < size; i++ )
			arr[ i ] = r->arr[ i ];
	}

public:

	/**
	 * @brief Construct a new empty record object
	 * 
	 */
	record( void )
	{}

	/**
	 * @brief writes data and makes copy if needed
	 * 
	 * @param add data to be writen
	 * @return record* instance pointer, either this or newly created, deep copy 
	 */
	record *Write( uint8_t add )
	{
		if( occurs > 1 ) {
			occurs--;
			
			return ( (new record( this ))->Write( add ) );
		}

		if( pos == size ) {
			if( allocated == size ) {
				allocated += PLUS;
				uint8_t *temp = new uint8_t[ allocated ];

				for( uint32_t i = 0; i < size; i++ )
					temp[ i ] = arr[ i ];

				delete[] arr;
				arr = temp;
			}
			size++;
		}
		arr[ pos++ ] = add;

		return this;
	}

	/**
	 * @brief reads the data from
	 * 
	 * @return uint8_t 
	 */
	uint8_t Read( void ) { return arr[ pos++ ]; }

    // prefix
    record & operator ++ ()
    {
		occurs ++;
        return *this;
    }

	record & operator -- ()
	{
		--occurs;
		return *this;
	}

	record *SetSize( uint32_t size )
	{
		if( occurs > 1 ) {
			record *r = new record( this );
			r->size = size;

			occurs--;
			return r;
		}
		else {
			this->size = size;
			return this;
		}
	}

	uint32_t GetSize( void ) { return size; }

	record *SetPos( uint32_t pos )
	{
		if( occurs > 1 ) {
			record *r = new record( this );
			r->pos = pos;

			occurs--;
			return r;
		}
		else {
			this->pos = pos;
			return this;
		}			
	}

	uint32_t GetPos( void ) { return pos; }

	uint32_t GetOccurs( void ) { return occurs; }

	void Print( void )
	{
		cout << endl;
		for( uint32_t i = 0; i < size; i++ )
			cout << (int)arr[ i ] << " ";
		cout << endl << pos << endl << occurs << endl << endl;
	}

	~record()
	{
		delete [] arr;
	}

};

//

class CFile
{
public:

	CFile( void )
	{
		size = 1;
		pos = 0;
		arr = new record*[1];
		arr[0] = new record();
	}

	CFile( const CFile & copy )
	{
		size = copy.size;
		pos = copy.pos;

		arr = new record*[ pos + 1 ];

		for( uint32_t i = 0; i <= pos; i++ ) {
			arr[ i ] = copy.arr[ i ];
			++( *arr[ i ] );
		}
	}

	~CFile( void )
	{
		for( uint32_t i = 0; i <= pos; i++ ) {
			delete arr[ i ];
			arr[ i ] = nullptr;
		}

		delete[] arr;
	}

	CFile &operator = (const CFile & assign)
	{
		arr = assign.arr;
		size = assign.size;
		pos = assign.pos;

		return *this;
	}

	bool seek( uint32_t offset )
	{
		if( offset > arr[ pos ]->GetSize() )
			return false;
		arr[ pos ] = arr[ pos ]->SetPos( offset );

		return true;
	}
	
	uint32_t read( uint8_t *dst,
				   uint32_t bytes )
	{
		for( uint32_t i = 0; i < bytes; i++ ) {
			record* here = arr[ pos ];
			if( here->GetPos() == here->GetSize() )
				return i;
			
			dst[ i ] = here->Read();
		}
		return bytes;
	}
	
	uint32_t write( const uint8_t *src,
				    uint32_t bytes )
	{
		for( uint32_t i = 0; i < bytes; i++ )
			arr[ pos ] = arr[ pos ]->Write( src[ i ] );

		return bytes;
	}

	void truncate( void )
	{
		arr[ pos ] = arr[ pos ]->SetSize( arr[ pos ]->GetPos() );
	}

	uint32_t fileSize( void ) const
	{
		return arr[ pos ]->GetSize();
	}

	void addVersion( void )
	{
		pos ++;
		if( pos == size ) {
			size += PLUS / 10;
			record **temp = new record* [ size ]();
			
			for( uint32_t i = 0; i < pos; i++ )
				temp[ i ] = arr[ i ];
			delete [] arr;
			arr = temp;
		}

		arr[ pos ] = arr[ pos - 1 ];
		++( *arr[ pos ] );
	}

	bool undoVersion( void )
	{
		if( pos == 0 )
			return false;

		if( ( --( *arr[ pos ] ) ).GetOccurs() == 0 )
			delete arr[ pos ];
		pos --;

		return true;
	}

	void Print( void )
	{
		arr[ pos ]->Print();
	}

private:

	record **arr = nullptr;
	uint32_t size = 0;
	uint32_t pos = 0;

};

//

#ifndef __PROGTEST__
bool writeTest( CFile &x,
			    const initializer_list<uint8_t> &data,
			    uint32_t wrLen )
{
	return x.write( data.begin(), data.size() ) == wrLen;
}

bool readTest( CFile &x,
			   const initializer_list<uint8_t> &data,
			   uint32_t rdLen )
{
	uint8_t tmp[100];
	uint32_t idx = 0;

	if (x.read(tmp, rdLen) != data.size())
		return false;
	for (auto v : data)
		if (tmp[idx++] != v)
			return false;
	return true;
}

int main( void )
{
	CFile f0;
	assert(writeTest(f0, {10, 20, 30}, 3));
	assert(f0.fileSize() == 3);
	assert(writeTest(f0, {60, 70, 80}, 3));
	assert(f0.fileSize() == 6);
	assert(f0.seek(2));
	assert(writeTest(f0, {5, 4}, 2));
	assert(f0.fileSize() == 6);
	assert(f0.seek(1));
	assert(readTest(f0, {20, 5, 4, 70, 80}, 7));
	assert(f0.seek(3));
	f0.addVersion();
	assert(f0.seek(6));
	assert(writeTest(f0, {100, 101, 102, 103}, 4));
	f0.addVersion();
	assert(f0.seek(5));
	CFile f1(f0);
	f0.truncate();
	assert(f0.seek(0));
	assert(readTest(f0, {10, 20, 5, 4, 70}, 20));
	assert(f0.undoVersion());
	assert(f0.seek(0));
	assert(readTest(f0, {10, 20, 5, 4, 70, 80, 100, 101, 102, 103}, 20));
	assert(f0.undoVersion());
	assert(f0.seek(0));
	assert(readTest(f0, {10, 20, 5, 4, 70, 80}, 20));
	assert(!f0.seek(100));
	assert(writeTest(f1, {200, 210, 220}, 3));
	assert(f1.seek(0));
	assert(readTest(f1, {10, 20, 5, 4, 70, 200, 210, 220, 102, 103}, 20));
	assert(f1.undoVersion());
	assert(f1.undoVersion());
	assert(readTest(f1, {4, 70, 80}, 20));
	assert(!f1.undoVersion());
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
