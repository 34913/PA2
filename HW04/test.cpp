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

	uint8_t *arr;

	uint32_t size = 0;
	uint32_t allocated = 0;

	uint32_t occurs;

public:

	record( void )
	:	arr( nullptr ),
		occurs( 0 )
	{}
	record( record *r )
	: occurs( 1 )
	{
		size = r->size;
		allocated = r->allocated;


		arr = new uint8_t[ allocated ];

		for( uint32_t i = 0; i < size; i++ )
			arr[ i ] = r->arr[ i ];
	}

	record *Write( uint8_t add, uint32_t pos )
	{
		record *use;
		if( occurs > 1 ) {
			occurs--;
			
			record *create = &record( this );

			use = create;
		}
		else
			use = this;

		if( pos == use->size ) {
			use->size++;

			if( use->allocated == use->size ) {
				use->allocated += PLUS;
				uint8_t *temp = new uint8_t[ use->allocated ];

				for( uint32_t i = 0; i < use->size; i++ )
					temp[ i ] = use->arr[ i ];

				delete[] use->arr;
				use->arr = temp;
			}
		}

		use->arr[ pos ] = add;

		return use;
	}

	uint8_t Read( uint32_t pos ) { return arr[ pos ]; }

	void AddOccurs( void ) { occurs++; }

};

//

/*
konstruktor
    implicitní konstruktor vytvoří prázdnou instanci souboru (velikost 0 B, pozice v 
	souboru 0).
destruktor, op= a kopírující konstruktor
    implementujte, pokud automaticky generovaná varianta nevyhovuje,
write (data, len)
    Metoda zapíše daný blok dat (data) o délce len na aktuální pozici. Aktuální 
	pozice v souboru se po zápisu posune za poslední zapsaný bajt. Metoda write 
	přepisuje data (je-li aktuální pozice uvnitř souboru)/rozšiřuje velikost souboru. 
	Návratovou hodnotou je počet zapsaných bajtů.
read (data, len)
    Metoda načte požadovaný počet bajtů (len) do pole data. Návratovou hodnotou 
	je počet načtených bajtů (může být menší než len podle aktuální pozice v souboru). 
	Metoda dále posune aktuální pozici v souboru vpřed o přečtený počet bajtů.
seek ( pos )
    metoda přesune aktuální pozici v souboru na pozici pos. Pozice se použije pro 
	následné operace čtení/zápisu. Parametr pos musí být v rozsahu 0 až délka souboru 
	(obě meze včetně). Návratovou hodnotou je true pro úspěch, false pro neúspěch 
	(pozice mimo meze).
truncate()
    metoda zkrátí soubor na velikost danou aktuální pozicí v souboru.
fileSize()
    metoda vrátí aktuální velikost souboru v bajtech.
addVersion()
    metoda archivuje aktuální obsah souboru a aktuální pozici v souboru 
	(vytvoří verzi). Tato verze bude uložena v instanci CFile.
undoVersion()
    metoda vrátí obsah souboru a aktuální pozici v souboru do stavu, ve kterém 
	byly při odpovídajícím předchozím volání addVersion. Vracet se k předchozím 
	verzím lze vícenásobně, dokud existují předchozí archivované verze. Volání 
	undoVersion vrátí true pro úspěch, false pro neúspěch (neexistuje předchozí verze).
*/

class CFile
{
public:
	CFile( void )
	:	size(0),
		pos(0)
	{}
	// copy cons, dtor, op=

	bool seek( uint32_t offset )
	{
		if( offset > size )
			return false;
		pos = offset;

		return true;
	}
	
	uint32_t read( uint8_t *dst,
				   uint32_t bytes );
	uint32_t write( const uint8_t *src,
				    uint32_t bytes );
	void truncate( void );
	uint32_t fileSize( void ) const;
	void addVersion( void );
	bool undoVersion( void );

private:
	// todo

	uint32_t pos;
	uint32_t size;


	//
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
