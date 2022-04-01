#ifndef __PROGTEST__
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <iostream>

using namespace std;
#endif /* __PROGTEST__ */

/**
 * @brief for saving the versions
 * 
 */
class record
{
private:

	uint32_t size = 0;
	uint32_t allocated = 0;
	uint32_t pos = 0;

	uint32_t occurs = 1;

	uint8_t *arr = nullptr;

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

		occurs = 1;

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
	 * @brief 
	 * 
	 * @param buffer 
	 * @param bytes 
	 * @return record* 
	 */
	record *Write( const uint8_t *buffer, uint32_t bytes )
	{
		if( occurs > 1 ) {
			occurs--;
			
			return ( (new record( this ))->Write( buffer, bytes ) );
		}

		if( size + bytes >= allocated ) {
			allocated += bytes + 10;
			uint8_t *temp = new uint8_t[ allocated ];

			for( uint32_t i = 0; i < size; i++ )
				temp[ i ] = arr[ i ];

			delete[] arr;
			arr = temp;
		}

		for( uint32_t i = 0; i < bytes; i++ ) {
			if( pos == size )
				size++;
			arr[ pos++ ] = buffer[ i ];
		}

		return this;
	}

	/**
	 * @brief reads the data from array based on position
	 * 
	 * @return uint8_t data read
	 */
	uint8_t Read( void ) { return arr[ pos++ ]; }

	/**
	 * @brief increseas occurs
	 * 
	 */
	void increaseOccurs( void ) { occurs++; }

	/**
	 * @brief decreseas occurs
	 * 
	 */
	void decreaseOccurs( void ) { occurs--; }

	/**
	 * @brief sets the new size of record, used for truncate the array
	 * creates deep copy if needed (its used somewhere else too)
	 * 
	 * @param size new size
	 * @return record* instance pointer, either this or newly created, deep copy
	 */
	record *SetSize( uint32_t size )
	{
		if( occurs > 1 ) {
			occurs--;
			return ( ( new record( this ) )->SetSize( size ) );
		}

		this->size = size;
		return this;
	}

	/**
	 * @brief sets the position in record, used for seek
	 * creates deep copy if needed (its used somewhere else too)
	 * 
	 * @param pos new position
	 * @return record* instance pointer, either this or newly created, deep copy
	 */
	record *SetPos( uint32_t pos )
	{
		if( occurs > 1 ) {
			occurs--;
			return ( ( new record( this ) )->SetPos( pos ) );
		}

		this->pos = pos;
		return this;
	}

	/**
	 * @brief gets the size of record
	 * 
	 * @return uint32_t size
	 */
	uint32_t GetSize( void ) { return size; }

	/**
	 * @brief gets the position of record
	 * 
	 * @return uint32_t position
	 */
	uint32_t GetPos( void ) { return pos; }

	/**
	 * @brief gets the occurences of record
	 * 
	 * @return uint32_t occurs
	 */
	uint32_t GetOccurs( void ) { return occurs; }

	/**
	 * @brief Destroy the record object and release used memory if not nullptr
	 * 
	 */
	~record()
	{
		delete [] arr;
	}

	void Print()
	{
		cout << endl;
		for( uint32_t i = 0; i < size; i++ )
			cout << (int)arr[ i ] << " ";
		cout << endl << pos << " " << occurs << endl << endl;
	}

};

//

class CFile
{
public:

	/**
	 * @brief Construct a new empty CFile object with one size
	 * 
	 */
	CFile( void )
	{
		size = 1;
		pos = 0;
		arr = new record* [ 1 ];
		arr[0] = new record;
	}

	/**
	 * @brief Construct a new copy of CFile object base on given CFile 
	 * 
	 * @param copy make copy of this
	 */
	CFile( const CFile & copy )
	{
		size = copy.pos + 1;
		pos = copy.pos;

		arr = new record* [ size ];

		for( uint32_t i = 0; i < size; i++ ) {
			arr[ i ] = copy.arr[ i ];
			arr[ i ]->increaseOccurs();
		}
	}

	/**
	 * @brief asssign copy
	 * 
	 * @param copy to be copied from
	 * @return CFile& the instance
	 */
	CFile & operator = ( const CFile & copy )
	{
		if( this == &copy )
			return *this;

		delete arr[ 0 ];
		delete [] arr;
		
		size = copy.pos + 1;
		pos = copy.pos;

		arr = new record* [ size ];

		for( uint32_t i = 0; i < size; i++ ) {
			arr[ i ] = copy.arr[ i ];
			arr[ i ]->increaseOccurs();
		}

		return *this;
	}

	/**
	 * @brief Destroy the CFile object and release any of allocated resources
	 * 
	 */
	~CFile( void )
	{
		for( uint32_t i = 0; i <= pos; i++ ) {
			arr[ i ]->decreaseOccurs();
			if( arr[ i ]->GetOccurs() == 0)
				delete arr[ i ];
		}

		delete[] arr;
	}

	/**
	 * @brief set position in record
	 * 
	 * @param offset position
	 * @return true on success
	 * @return false on failure
	 */
	bool seek( uint32_t offset )
	{
		if( offset > arr[ pos ]->GetSize() )
			return false;
		arr[ pos ] = arr[ pos ]->SetPos( offset );

		return true;
	}
	
	/**
	 * @brief read postion of data from last version from position set by seek()
	 * 
	 * @param dst buffer to save data to
	 * @param bytes number of bytes to be read
	 * @return uint32_t number of bytes actually read
	 */
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
	
	/**
	 * @brief write specified number of bytes in last version
	 * 
	 * @param src buffer of data to be written
	 * @param bytes number of bytes
	 * @return uint32_t number of bytes actually written
	 */
	uint32_t write( const uint8_t *src,
				    uint32_t bytes )
	{
		arr[ pos ] = arr[ pos ]->Write( src, bytes );

		return bytes;
	}

	/**
	 * @brief truncate the size based on position
	 * 
	 */
	void truncate( void )
	{
		arr[ pos ] = arr[ pos ]->SetSize( arr[ pos ]->GetPos() );
	}

	/**
	 * @brief gets the size of last version
	 * 
	 * @return uint32_t size
	 */
	uint32_t fileSize( void ) const
	{
		return arr[ pos ]->GetSize();
	}

	/**
	 * @brief adds version of record in array
	 * makes copy, save it and add occurs
	 * 
	 */
	void addVersion( void )
	{
		pos ++;
		if( pos == size ) {
			size += 10;
			record **temp = new record* [ size ];
			
			for( uint32_t i = 0; i < pos; i++ )
				temp[ i ] = arr[ i ];
			delete [] arr;
			arr = temp;
		}

		arr[ pos ] = arr[ pos - 1 ];
		arr[ pos ]->increaseOccurs();
	}

	/**
	 * @brief tracks back one version and deletes the last if needed
	 * (if its reference is not anywhere else)
	 * 
	 * @return true on success
	 * @return false on failure
	 */
	bool undoVersion( void )
	{
		if( pos == 0 )
			return false;

		arr[ pos ]->decreaseOccurs();
		if( arr[ pos ]->GetOccurs() == 0 )
			delete arr[ pos ];
		pos --;

		return true;
	}

	void Print()
	{
		arr[ pos ]->Print();
	}

private:

	record **arr = nullptr;
	
	// allocated size
	uint32_t size = 0;

	// position in versions
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
	// CFile f1(f0);
	CFile f1;
	f1 = f0;
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

	CFile f2;
	f2.addVersion();
	assert(f2.undoVersion());
	assert(!f2.undoVersion());

	assert(writeTest(f2,{1,2,3,4},4));
	assert(f2.seek(2));
	f2.truncate();
	assert(!f2.seek(3));
	assert(writeTest(f2,{3},1));
	assert(f2.seek(0));
	assert(readTest(f2,{1,2,3},3));
	assert(!f2.undoVersion());
	assert(f2.seek(0));
	f2.truncate();
	assert(!f2.seek(1));
	assert(writeTest(f2,{0,1},2));
	assert(f2.seek(1));
	assert(readTest(f2,{1},1));

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
