#ifndef __PROGTEST__
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

//=================================================================================================
// a dummy exception class, keep this implementation
class InvalidDateException : public invalid_argument
{
public:
	InvalidDateException()
		: invalid_argument("invalid date or format")
	{
	}
};

//=================================================================================================
// date_format manipulator - a dummy implementation. Keep this code unless you implement your
// own working manipulator.
ios_base &(*date_format(const char *fmt))(ios_base &x)
{
	return [](ios_base &ios) -> ios_base &
	{ return ios; };
}
//=================================================================================================

class CDate
{
	time_t date;
	// tm *date;

public:

    // konstruktor s parametry (y,m,d) vytvoří novou instanci třídy s datem nastaveným podle předaných hodnot. Konstruktor musí kontrolovat, zda zadané datum je platné. Pokud platné není, musí vyvolat výjimku InvalidDateException,
    // operátorem + lze k instanci CDate přičíst celé číslo a tím se posunout vpřed o zadaný počet dní (vzad pro záporné číslo),
	// operátorem - lze od instance CDate odečíst celé číslo a tím se posunout vzad o zadaný počet dní (vpřed pro záporné číslo),
	//
    // operátorem - lze od sebe odečíst dvě instance CDate a tím získat počet dní mezi těmito dvěma daty,
    // operátory ++ a -- v prefixové a v postfixové notaci lze zvyšovat/snižovat datum o 1 den, operátory mají obvyklé chování,
    // operátory ==, !=, >, >=, < a <= lze porovnávat dvojici instancí CDate, v této relaci budou data v budoucnosti větší než data v minulosti,
    // operátorem << lze zobrazit instanci CDate v zadaném streamu. Při zobrazování se používá ISO formát (%Y-%m-%d, tedy např. 2000-01-31). V povinných testech bude vždy použit tento implicitní formát. Bonusové testy požadují implementaci manipulátoru date_format, kterým lze formát řídit.
    // operátorem >> lze přečíst instanci CDate ze zadaného streamu. V povinných testech je na vstupu očekáváno datum v ISO formátu %Y-%m-%d. Pokud se nepodaří datum načíst (formát, neplatné datum, ...), operátor zajistí nastavení fail bitu a ponechá původní obsah instance CDate. Stejně jako výstupní operátor, i vstup lze řídit pomocí manipulátoru date_format, tato možnost je požadovaná v bonusovém testu.

	CDate( int y, int m, int d )
	{
		tm *temp = new tm();
		temp->tm_mday = d;
		temp->tm_mon = m - 1;
		temp->tm_year = y - 1900;

		date = mktime( temp );

		tm *tempTM = localtime( &date );

		if( tempTM->tm_year != temp->tm_year
			|| tempTM->tm_mon != temp->tm_mon
			|| tempTM->tm_mday != temp->tm_mday )
			throw new InvalidDateException();

		delete temp;
	}

	CDate( tm *t )
	{
		date = mktime( t );
	}
	
	CDate( time_t t )
	:	date( t )
	{}

	int operator - ( const CDate x ) const
	{
		return difftime( date, x.date ) / 86400;
	}

	CDate operator + ( int days )
	{
		if( days < 0 )
			return *this - abs( days );

		return CDate( date + days * 86400 );
	}

	CDate operator - ( int days )
	{
		if( days < 0 )
			return *this + abs( days );

		return CDate( date - days * 86400 );
	}

	bool operator ==( const CDate & x )
	{
		return ( date == x.date );
	}

	bool operator < ( const CDate & x )
	{
		return date < x.date;
	}

	bool operator > ( const CDate & x )
	{
		return date > x.date;
	}

	bool operator != ( const CDate & x )
	{
		return ( date != x.date );
	}

	bool operator >= ( const CDate & x )
	{
		return !( *this < x );
	}

	bool operator <= ( const CDate & x )
	{
		return !( *this > x );
	}

    // prefix
    CDate & operator ++()
    {
		date += 86400;
        return *this;
    }
 
    // postfix
    CDate operator ++( int )
    {
        CDate old = *this;
        operator++();
        return old;
    }

    // prefix
    CDate & operator --()
    {
		date -= 86400;
        return *this;
    }
 
    // postfix
    CDate operator --( int )
    {
        CDate old = *this;
        operator--();
        return old;
    }

	friend ostream & operator << ( ostream & os, const CDate x )
	{
		tm *temp = localtime( &x.date );
		stringstream ss("");
		ss << std::setw( 4 ) << std::setfill( '0' ) << 1900 + temp->tm_year << '-'
			<< std::setw( 2 ) << std::setfill( '0' ) << temp->tm_mon + 1 << '-'
			<<std::setw( 2 ) << std::setfill( '0' ) << temp->tm_mday;
		os << ss.str();
		os.flush();
		cout << ss.str() << endl;
		return os;
	}

	friend istream & operator >>( istream & is, CDate & x )
	{
		string str;
		is >> str;

		int arr[ 3 ] = { 0, 0, 0 };

		int last = 0;
		int delimiter;
		string sub;
		for( int i = 0; i < 3; i++ ) {
			delimiter = str.find( '-', last );
			if( delimiter == -1 )
				delimiter = str.length() - last;
			else
				delimiter -= last;
			sub = str.substr( last, delimiter );

			arr[ i ] = atoi( sub.c_str() );

			last = delimiter + last + 1;
		}

		try {
			x = CDate( arr[ 0 ], arr[ 1 ], arr[ 2 ] );
		}
		catch( ... ) {
			// error
			if( false )
				is.setstate(std::ios::failbit);
		}
		
		return is;
	}

};

#ifndef __PROGTEST__
int main(void)
{
	ostringstream oss;
	istringstream iss;

	CDate a(2000, 1, 2);
	CDate b(2010, 2, 3);
	CDate c(2004, 2, 10);
	oss.str("");
	oss << a;
	assert(oss.str() == "2000-01-02");
	oss.str("");
	oss << b;
	assert(oss.str() == "2010-02-03");
	oss.str("");
	oss << c;
	assert(oss.str() == "2004-02-10");

	a = a + 1500;
	oss.str("");
	oss << a;
	assert(oss.str() == "2004-02-10");
	b = b - 2000;
	oss.str("");
	oss << b;
	assert(oss.str() == "2004-08-13");

	assert(b - a == 185);
	assert((b == a) == false);
	assert((b != a) == true);
	assert((b <= a) == false);
	assert((b < a) == false);
	assert((b >= a) == true);
	assert((b > a) == true);
	assert((c == a) == true);
	assert((c != a) == false);
	assert((c <= a) == true);
	assert((c < a) == false);
	assert((c >= a) == true);
	assert((c > a) == false);

	a = ++c;
	oss.str("");
	oss << a << " " << c;
	assert(oss.str() == "2004-02-11 2004-02-11");
	a = --c;
	oss.str("");
	oss << a << " " << c;
	assert(oss.str() == "2004-02-10 2004-02-10");
	a = c++;
	oss.str("");
	oss << a << " " << c;
	assert(oss.str() == "2004-02-10 2004-02-11");
	a = c--;
	oss.str("");
	oss << a << " " << c;
	assert(oss.str() == "2004-02-11 2004-02-10");
	iss.clear();
	iss.str("2015-09-03");
	assert((iss >> a));
	oss.str("");
	oss << a;
	assert(oss.str() == "2015-09-03");
	a = a + 70;
	oss.str("");
	oss << a;
	assert(oss.str() == "2015-11-12");

	CDate d(2000, 1, 1);
	try
	{
		CDate e(2000, 32, 1);
		assert("No exception thrown!" == nullptr);
	}
	catch (...)
	{
	}
	iss.clear();
	iss.str("2000-12-33");
	assert(!(iss >> d));
	oss.str("");
	oss << d;
	assert(oss.str() == "2000-01-01");
	iss.clear();
	iss.str("2000-11-31");
	assert(!(iss >> d));
	oss.str("");
	oss << d;
	assert(oss.str() == "2000-01-01");
	iss.clear();
	iss.str("2000-02-29");
	assert((iss >> d));
	oss.str("");
	oss << d;
	assert(oss.str() == "2000-02-29");
	iss.clear();
	iss.str("2001-02-29");
	assert(!(iss >> d));
	oss.str("");
	oss << d;
	assert(oss.str() == "2000-02-29");

	/*

	//-----------------------------------------------------------------------------
	// bonus test examples
	//-----------------------------------------------------------------------------
	CDate f(2000, 5, 12);
	oss.str("");
	oss << f;
	assert(oss.str() == "2000-05-12");
	oss.str("");
	oss << date_format("%Y/%m/%d") << f;
	assert(oss.str() == "2000/05/12");
	oss.str("");
	oss << date_format("%d.%m.%Y") << f;
	assert(oss.str() == "12.05.2000");
	oss.str("");
	oss << date_format("%m/%d/%Y") << f;
	assert(oss.str() == "05/12/2000");
	oss.str("");
	oss << date_format("%Y%m%d") << f;
	assert(oss.str() == "20000512");
	oss.str("");
	oss << date_format("hello kitty") << f;
	assert(oss.str() == "hello kitty");
	oss.str("");
	oss << date_format("%d%d%d%d%d%d%m%m%m%Y%Y%Y%%%%%%%%%%") << f;
	assert(oss.str() == "121212121212050505200020002000%%%%%");
	oss.str("");
	oss << date_format("%Y-%m-%d") << f;
	assert(oss.str() == "2000-05-12");
	iss.clear();
	iss.str("2001-01-1");
	assert(!(iss >> f));
	oss.str("");
	oss << f;
	assert(oss.str() == "2000-05-12");
	iss.clear();
	iss.str("2001-1-01");
	assert(!(iss >> f));
	oss.str("");
	oss << f;
	assert(oss.str() == "2000-05-12");
	iss.clear();
	iss.str("2001-001-01");
	assert(!(iss >> f));
	oss.str("");
	oss << f;
	assert(oss.str() == "2000-05-12");
	iss.clear();
	iss.str("2001-01-02");
	assert((iss >> date_format("%Y-%m-%d") >> f));
	oss.str("");
	oss << f;
	assert(oss.str() == "2001-01-02");
	iss.clear();
	iss.str("05.06.2003");
	assert((iss >> date_format("%d.%m.%Y") >> f));
	oss.str("");
	oss << f;
	assert(oss.str() == "2003-06-05");
	iss.clear();
	iss.str("07/08/2004");
	assert((iss >> date_format("%m/%d/%Y") >> f));
	oss.str("");
	oss << f;
	assert(oss.str() == "2004-07-08");
	iss.clear();
	iss.str("2002*03*04");
	assert((iss >> date_format("%Y*%m*%d") >> f));
	oss.str("");
	oss << f;
	assert(oss.str() == "2002-03-04");
	iss.clear();
	iss.str("C++09format10PA22006rulez");
	assert((iss >> date_format("C++%mformat%dPA2%Yrulez") >> f));
	oss.str("");
	oss << f;
	assert(oss.str() == "2006-09-10");
	iss.clear();
	iss.str("%12%13%2010%");
	assert((iss >> date_format("%%%m%%%d%%%Y%%") >> f));
	oss.str("");
	oss << f;
	assert(oss.str() == "2010-12-13");

	CDate g(2000, 6, 8);
	iss.clear();
	iss.str("2001-11-33");
	assert(!(iss >> date_format("%Y-%m-%d") >> g));
	oss.str("");
	oss << g;
	assert(oss.str() == "2000-06-08");
	iss.clear();
	iss.str("29.02.2003");
	assert(!(iss >> date_format("%d.%m.%Y") >> g));
	oss.str("");
	oss << g;
	assert(oss.str() == "2000-06-08");
	iss.clear();
	iss.str("14/02/2004");
	assert(!(iss >> date_format("%m/%d/%Y") >> g));
	oss.str("");
	oss << g;
	assert(oss.str() == "2000-06-08");
	iss.clear();
	iss.str("2002-03");
	assert(!(iss >> date_format("%Y-%m") >> g));
	oss.str("");
	oss << g;
	assert(oss.str() == "2000-06-08");
	iss.clear();
	iss.str("hello kitty");
	assert(!(iss >> date_format("hello kitty") >> g));
	oss.str("");
	oss << g;
	assert(oss.str() == "2000-06-08");
	iss.clear();
	iss.str("2005-07-12-07");
	assert(!(iss >> date_format("%Y-%m-%d-%m") >> g));
	oss.str("");
	oss << g;
	assert(oss.str() == "2000-06-08");
	iss.clear();
	iss.str("20000101");
	assert((iss >> date_format("%Y%m%d") >> g));
	oss.str("");
	oss << g;
	assert(oss.str() == "2000-01-01");
*/

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
