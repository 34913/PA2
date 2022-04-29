#ifndef __PROGTEST__
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stdexcept>
#include <algorithm>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

//

class CInterface
{
public:
	const int id;
	const string tName;

	CInterface( int id, string type )
	:	id(id), tName( type )
	{}

	virtual size_t getSize() const = 0;

	virtual bool operator==( const CInterface & x ) const
    {
		return this->id == x.id;
    }
	
	bool operator!=( const CInterface & x ) const
	{
		return !( *this == x );
	}

	virtual void print( ostream & os ) const
	{
		os << tName;
	}

	friend ostream & operator << ( ostream & os, const CInterface & x )
	{
		x.print( os );
		return os;
	}

	virtual CInterface & field( const string & name ) const
	{
		stringstream ss;
		ss << "Cannot use field() for type: ";
		ss << *this;
		throw invalid_argument( ss.str() );
	}

	virtual CInterface & element() const
	{
		stringstream ss;
		ss << "Cannot use element() for type: ";
		ss << *this;
		throw invalid_argument( ss.str() );
	}

	virtual CInterface * clone() const = 0;

	virtual ~CInterface() {}

};

class CDataTypeInt: public CInterface
{
public:

	CDataTypeInt()
	:	CInterface( 1, "int" )
	{}

	size_t getSize() const override
	{
		return 4;
	}

	CDataTypeInt * clone() const override
	{
		return new CDataTypeInt ( *this );
	}

};
class CDataTypeDouble: public CInterface
{
public:
	CDataTypeDouble()
	:	CInterface( 2, "double" )
	{}

    size_t getSize() const override
    {
        return 8;
    }

	CDataTypeDouble * clone() const override
	{
		return new CDataTypeDouble( *this );
	}

};

class CDataTypeEnum: public CInterface
{
private:
	vector<string> types;

public:

	CDataTypeEnum()
	:	CInterface( 3, "enum" )
	{}

	size_t getSize() const override
	{
		return 4;
	}

	CDataTypeEnum & add( const string & name )
	{
		for( size_t i = 0; i < types.size(); i++ ) {
			if( types[ i ] == name ) {
				string str = "Duplicate enum value: ";
				str += name;
				throw invalid_argument( str );
			}
		}

		string copy = name;
		types.push_back( copy );
		return *this;
	}

	CDataTypeEnum & add( const char * name )
	{
		string str = name;
		return add( str );
	}

	bool operator==( const CInterface & x ) const override
	{
		if( this->id != x.id )
			return false;

		CDataTypeEnum *data = (CDataTypeEnum*) &x;

		if( data->types.size() != types.size() )
			return false;

		for( size_t i = 0; i < types.size(); i++ ) {
			if( types[ i ].compare( data->types[ i ] ) != 0 )
				return false;
		}
		return true;
	}

	void print( ostream & os ) const override
	{
		os << tName << endl << "{" << endl;
		for( size_t i = 0; i < types.size(); i++ ) {
			os << types[ i ];
			if( i + 1 == types.size() )
				os << endl;
			else
				os << "," << endl;
		}
		os << "}";
	}

	CDataTypeEnum * clone() const override
	{
		return new CDataTypeEnum( *this );
	}

};
class CDataTypeStruct: public CInterface
{
private:
	vector<pair<string, shared_ptr<CInterface>>> content;

public:

	CDataTypeStruct()
	:	CInterface( 4, "struct" )
	{}

	size_t getSize() const override
	{
		size_t size = 0;
		for( size_t i = 0; i < content.size(); i++ )
			size += content[ i ].second->getSize();
		return size;
	}

	CDataTypeStruct & addField( const string & name, const CInterface & type )
	{
		for( size_t i = 0; i < content.size(); i++ ) {
			if( name == content[ i ].first ) {
				string str = "Duplicate field: " + name;
				throw invalid_argument( str );
			}
		}

		shared_ptr<CInterface> copy( type.clone() );
		content.push_back( { name, copy } );

		return *this;
	}

	CInterface & field( const string & name ) const override
	{
		for( size_t i = 0; i < content.size(); i++ )
		{
			if( name == content[ i ].first )
				return *content[ i ].second;
		}

		string str = "Unknown field: ";
		str += name;
		throw invalid_argument( str );
	}

	bool operator==( const CInterface & x ) const override
	{
		if( this->id != x.id )
			return false;

		CDataTypeStruct *data = (CDataTypeStruct*) &x;
		if( data->content.size() != content.size() )
			return false;

		for( size_t i = 0; i < content.size(); i++ ) {
			if( *data->content[ i ].second != *content[ i ].second )
				return false;
		}
		return true;
	}

	void print( ostream & os ) const override
	{
		os << tName << endl << "{" << endl;

		for( size_t i = 0; i < content.size(); i++ )
			os << *content[ i ].second << " " << content[ i ].first << ";" << endl;
		
		os << "}";
	}

	CDataTypeStruct * clone() const override
	{
		return new CDataTypeStruct( *this );
	}

};

class CDataTypePtr: public CInterface
{
private:

	shared_ptr<CInterface> type;

public:

	CDataTypePtr( const CInterface & x )
	:	CInterface( 6, "ptr" ), type( x.clone() )
	{}

	size_t getSize() const override
	{
		return 8;
	}

	CInterface * clone() const override
	{
		return new CDataTypePtr( *this );
	}

	CInterface & element() const override
	{
		return *type;
	}

	void print( ostream & os ) const override;

	pair<string, CInterface*> printBrackets() const;

	bool operator==( const CInterface & x ) const override
	{
		if( this->id != x.id )
			return false;
		return *type == x;
	}
};
class CDataTypeArray: public CInterface
{
protected:

	pair<size_t, shared_ptr<CInterface>> content;

public:

	CDataTypeArray( size_t size, const CInterface & type )
	:	CInterface( 5, "array" )
	{
		content = { size, shared_ptr<CInterface>( type.clone() ) };
	}

	size_t getSize() const override
	{
		return content.first * content.second->getSize();
	}

	CInterface * clone() const override
	{
		return new CDataTypeArray( *this );
	}

	CInterface & element() const override
	{
		return *content.second;
	}

	void print( ostream & os ) const override
	{
		string brackets = "";
		CDataTypeArray *ptr = ( CDataTypeArray* )this;

		while( true ) {
			brackets += "[" + to_string( ptr->content.first ) + "]";

			if( ptr->content.second->id != 5 )
				break;
			ptr = ( CDataTypeArray* ) ptr->content.second.get();
		}

		if( ptr->content.second->id == 6 ) {
			pair<string, CInterface*> back = ( ( CDataTypePtr* )ptr->content.second.get() )->printBrackets();

			os << *back.second << back.first << brackets;
		}
		else {
			os << *ptr->content.second << brackets;
		}
	}

	pair<string, CInterface*> printSize() const
	{
		string str = "[" + to_string( content.first ) + "]";

		if( content.second->id == 5 ) {
			pair<string, CInterface*> p = ( ( CDataTypeArray* ) content.second.get() )->printSize();
			return { str + p.first, p.second };
		}
		else if( content.second->id == 6 ) {
			pair<string, CInterface*> p = ( ( CDataTypePtr* )content.second.get() )->printBrackets();
			return { p.first + str, p.second };
		}
		
		// while( true ) {
		// 	ss << "[" << to_string( ptr->content.first ) << "]";
		// 	// if( ptr->content.second->id == 6 ) {
		// 	// 	ostringstream special;
		// 	// 	CDataTypePtr* now = ( CDataTypePtr* ) ptr->content.second.get();
		// 	// 	special << *( ( CInterface* ) now );
		// 	// 	CInterface * next = ( CInterface* ) 
		// 	// 	return { special.str() + ss.str(), next };
		// 	// }
		// 	if( ptr->content.second->id != 5 )
		// 		break;
		// 	ptr = ( CDataTypeArray* ) ptr->content.second.get();
		// }

		return { str, content.second.get() };
	}

	bool operator==( const CInterface & x ) const override
	{
		if( this->id != x.id )
			return false;

		CDataTypeArray *data = (CDataTypeArray*) &x;
		if( data->content.first != content.first || *data->content.second != *content.second )
			return false;
		
		return true;
	}

};

void CDataTypePtr::print( ostream & os ) const
{
	CInterface * ptr = ( CInterface* ) this->type.get();

	if( ptr->id == 5 ) {
		// pair<string, CInterface*> back = ( ( CDataTypeArray* ) ptr )->printSize();
		pair<string, CInterface*> back = printBrackets();
		os << *back.second << "*" << back.first;
	}
	else {
		os << *type << "*";
	}
}

pair<string, CInterface*> CDataTypePtr::printBrackets() const
{
	if( type->id == 5 ) {
		pair<string, CInterface*> p = ( ( CDataTypeArray* ) type.get() )->printSize();
		string str = "(" + p.first + ")";
		return { str, p.second };
	}

	string str = "*";
	return { str, type.get() };
}

#ifndef __PROGTEST__
static bool whitespaceMatch(const string &a,
							const string &b)
{
    int posA = 0;
    int posB = 0;

    while( true ) {
        if( a[ posA ] == '\0' && b[ posB ] == '\0' )
            return true;
        if( a[ posA ] == '\0' )
            return false;
        if( b[ posB ] == '\0' )
            return false;

        if( a[ posA ] == ' ' || a[ posA ] == '\t' || a[ posA ] == '\n' ) {
            posA++;
            continue;
        }
        
        if( b[ posB ] == ' ' || b[ posB ] == '\t' || b[ posB ] == '\n' ) {
            posB++;
            continue;
        }

        if( a[ posA ] != b[ posB ] )
            return false;

		posA++;
		posB++;
    }

	return true;
}
template <typename T_>
static bool whitespaceMatch(const T_ &x,
							const string &ref)
{
	ostringstream oss;
	oss << x;
	return whitespaceMatch(oss.str(), ref);
}

//

int main(void)
{

  CDataTypeStruct  a = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct b = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "READY" ) ).
                        addField ( "m_Ratio", CDataTypeDouble () );
  
  CDataTypeStruct c =  CDataTypeStruct () .
                        addField ( "m_First", CDataTypeInt () ) .
                        addField ( "m_Second", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Third", CDataTypeDouble () );

  CDataTypeStruct  d = CDataTypeStruct () .
                        addField ( "m_Length", CDataTypeInt () ) .
                        addField ( "m_Status", CDataTypeEnum () . 
                          add ( "NEW" ) . 
                          add ( "FIXED" ) . 
                          add ( "BROKEN" ) . 
                          add ( "DEAD" ) ).
                        addField ( "m_Ratio", CDataTypeInt () );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( c, "struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "}") );

  assert ( whitespaceMatch ( d, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  int m_Ratio;\n"
    "}") );

  assert ( a != b );
  assert ( a == c );
  assert ( a != d );
  assert ( a != CDataTypeInt() );
  assert ( whitespaceMatch ( a . field ( "m_Status" ), "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  DEAD\n"
    "}") );

  b . addField ( "m_Other", a );

  a . addField ( "m_Sum", CDataTypeInt ());

  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  int m_Sum;\n"
    "}") );

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      DEAD\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "  } m_Other;\n"
    "}") );

  assert ( whitespaceMatch ( b . field ( "m_Other" ), "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "}") );

  assert ( whitespaceMatch ( b . field ( "m_Other" ) . field ( "m_Status" ), "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  DEAD\n"
    "}") );

  assert ( a . getSize () == 20 );
  assert ( b . getSize () == 32 );
  b . addField ( "m_Other1", b );
  b . addField ( "m_Other2", b );
  b . addField ( "m_Other3", b );
  assert ( b . field ( "m_Other3" ) . field ( "m_Other2" ) . field ( "m_Other1" ) == b . field ( "m_Other1" ) );

  assert ( b . getSize () == 256);

  assert ( whitespaceMatch ( b, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      DEAD\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "  } m_Other;\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      READY\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        DEAD\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "    } m_Other;\n"
    "  } m_Other1;\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      READY\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        DEAD\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "    } m_Other;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        READY\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "      struct\n"
    "      {\n"
    "        int m_Length;\n"
    "        enum\n"
    "        {\n"
    "          NEW,\n"
    "          FIXED,\n"
    "          BROKEN,\n"
    "          DEAD\n"
    "        } m_Status;\n"
    "        double m_Ratio;\n"
    "      } m_Other;\n"
    "    } m_Other1;\n"
    "  } m_Other2;\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      READY\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        DEAD\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "    } m_Other;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        READY\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "      struct\n"
    "      {\n"
    "        int m_Length;\n"
    "        enum\n"
    "        {\n"
    "          NEW,\n"
    "          FIXED,\n"
    "          BROKEN,\n"
    "          DEAD\n"
    "        } m_Status;\n"
    "        double m_Ratio;\n"
    "      } m_Other;\n"
    "    } m_Other1;\n"
    "    struct\n"
    "    {\n"
    "      int m_Length;\n"
    "      enum\n"
    "      {\n"
    "        NEW,\n"
    "        FIXED,\n"
    "        BROKEN,\n"
    "        READY\n"
    "      } m_Status;\n"
    "      double m_Ratio;\n"
    "      struct\n"
    "      {\n"
    "        int m_Length;\n"
    "        enum\n"
    "        {\n"
    "          NEW,\n"
    "          FIXED,\n"
    "          BROKEN,\n"
    "          DEAD\n"
    "        } m_Status;\n"
    "        double m_Ratio;\n"
    "      } m_Other;\n"
    "      struct\n"
    "      {\n"
    "        int m_Length;\n"
    "        enum\n"
    "        {\n"
    "          NEW,\n"
    "          FIXED,\n"
    "          BROKEN,\n"
    "          READY\n"
    "        } m_Status;\n"
    "        double m_Ratio;\n"
    "        struct\n"
    "        {\n"
    "          int m_Length;\n"
    "          enum\n"
    "          {\n"
    "            NEW,\n"
    "            FIXED,\n"
    "            BROKEN,\n"
    "            DEAD\n"
    "          } m_Status;\n"
    "          double m_Ratio;\n"
    "        } m_Other;\n"
    "      } m_Other1;\n"
    "    } m_Other2;\n"
    "  } m_Other3;\n"
    "}" ) );



  try
  {
    a . addField ( "m_Status", CDataTypeInt () );
    assert ( "addField: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate field: m_Status"s );
  }

  try
  {
    cout << a . field ( "m_Fail" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Unknown field: m_Fail"s );
  }

  try
  {
    cout << a . field ( "m_Length" ) . field ( "m_Foo" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( whitespaceMatch ( e . what (), "Cannot use field() for type: int" ));
  }

  try
  {
    cout << a . field ( "m_Status" ) . field ( "m_Foo" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( whitespaceMatch ( e . what (), "Cannot use field() for type: enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  DEAD\n"
    "}" ));
  }

  try
  {
    CDataTypeEnum en;
    en . add ( "FIRST" ) .
         add ( "SECOND" ) .
         add ( "FIRST" );
    assert ( "add: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( e . what () == "Duplicate enum value: FIRST"s );
  }

  CDataTypeArray ar1 ( 10, CDataTypeInt () );
  assert ( whitespaceMatch ( ar1, "int[10]") );
  assert ( whitespaceMatch ( ar1 . element (), "int") );
  CDataTypeArray ar2 ( 11, ar1 );
  assert ( whitespaceMatch ( ar2, "int[11][10]") );
  assert ( whitespaceMatch ( ar2 . element (), "int[10]") );
  assert ( whitespaceMatch ( ar2 . element () . element (), "int") );
  CDataTypeArray ar3 ( 10, CDataTypeArray ( 20, CDataTypePtr ( CDataTypeInt () ) ) );
  cout << ar3 << endl;
  assert ( whitespaceMatch ( ar3, "int*[10][20]") );
  assert ( whitespaceMatch ( ar3 . element (), "int*[20]") );
  assert ( whitespaceMatch ( ar3 . element () . element (), "int*") );
  assert ( whitespaceMatch ( ar3 . element () . element () . element (), "int") );
  CDataTypePtr ar4  ( ar1 . element () );
  assert ( whitespaceMatch ( ar4, "int*") );
  assert ( whitespaceMatch ( ar4 . element (), "int") );
  CDataTypePtr ar5  ( b . field ( "m_Status" ) );
  assert ( whitespaceMatch ( ar5, "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  READY\n"
    "}*") );
  assert ( whitespaceMatch ( ar5 . element (), "enum\n"
    "{\n"
    "  NEW,\n"
    "  FIXED,\n"
    "  BROKEN,\n"
    "  READY\n"
    "}") );
  CDataTypePtr ar6 ( ar3 . element () . element () );
  cout << ar6 << endl;
  assert ( whitespaceMatch ( ar6, "int**") );
  assert ( whitespaceMatch ( ar6 . element (), "int*") );
  assert ( whitespaceMatch ( ar6 . element () . element (), "int") );
  CDataTypePtr ar7 ( CDataTypeArray ( 50, ar6 ) );
  cout << ar7 << endl;
  assert ( whitespaceMatch ( ar7, "int**(*)[50]") );
  assert ( whitespaceMatch ( ar7 . element (), "int**[50]") );
  assert ( whitespaceMatch ( ar7 . element () . element (), "int**") );
  assert ( whitespaceMatch ( ar7 . element () . element () . element (), "int*") );
  assert ( whitespaceMatch ( ar7 . element () . element () . element () . element (), "int") );
  CDataTypeArray ar8 ( 25, ar7 );
  cout << ar8 << endl;
  assert ( whitespaceMatch ( ar8, "int**(*[25])[50]") );
  CDataTypePtr ar9 ( ar8 );
  assert ( whitespaceMatch ( ar9, "int**(*(*)[25])[50]") );
  // int** -> int**(*)[50] -> int**(*[25])[50] -> int**(*(*)[25])[50]
  a . addField ( "m_Ar1", ar1 ) .
      addField ( "m_Ar2", ar2 ) .
      addField ( "m_Ar3", ar3 ) .
      addField ( "m_Ar4", ar4 ) .
      addField ( "m_Ar5", ar5 ) .
      addField ( "m_Ar6", ar6 ) .
      addField ( "m_Ar7", ar7 ) .
      addField ( "m_Ar8", ar8 ) .
      addField ( "m_Ar9", ar9 );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  int m_Sum;\n"
    "  int m_Ar1[10];\n"
    "  int m_Ar2[11][10];\n"
    "  int* m_Ar3[10][20];\n"
    "  int* m_Ar4;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  }* m_Ar5;\n"
    "  int** m_Ar6;\n"
    "  int**(* m_Ar7)[50];\n"
    "  int**(* m_Ar8[25])[50];\n"
    "  int**(*(* m_Ar9)[25])[50];\n"
    "}") );
  a . addField ( "m_Madness", CDataTypeArray ( 40, CDataTypePtr ( a ) ) );
  assert ( whitespaceMatch ( a, "struct\n"
    "{\n"
    "  int m_Length;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Status;\n"
    "  double m_Ratio;\n"
    "  int m_Sum;\n"
    "  int m_Ar1[10];\n"
    "  int m_Ar2[11][10];\n"
    "  int* m_Ar3[10][20];\n"
    "  int* m_Ar4;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    READY\n"
    "  }* m_Ar5;\n"
    "  int** m_Ar6;\n"
    "  int**(* m_Ar7)[50];\n"
    "  int**(* m_Ar8[25])[50];\n"
    "  int**(*(* m_Ar9)[25])[50];\n"
    "  struct\n"
    "  {\n"
    "    int m_Length;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      DEAD\n"
    "    } m_Status;\n"
    "    double m_Ratio;\n"
    "    int m_Sum;\n"
    "    int m_Ar1[10];\n"
    "    int m_Ar2[11][10];\n"
    "    int* m_Ar3[10][20];\n"
    "    int* m_Ar4;\n"
    "    enum\n"
    "    {\n"
    "      NEW,\n"
    "      FIXED,\n"
    "      BROKEN,\n"
    "      READY\n"
    "    }* m_Ar5;\n"
    "    int** m_Ar6;\n"
    "    int**(* m_Ar7)[50];\n"
    "    int**(* m_Ar8[25])[50];\n"
    "    int**(*(* m_Ar9)[25])[50];\n"
    "  }* m_Madness[40];\n"
    "}") );
  assert ( a . field ( "m_Madness" ) . element () . element () . field ( "m_Ar9" ) == a . field ( "m_Ar9" ));
  assert ( a . field ( "m_Madness" ) . element () . element () . field ( "m_Ar9" ) != a . field ( "m_Ar8" ));
  try
  {
    cout << ar2 . field ( "m_Foo" ) << endl;
    assert ( "field: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( whitespaceMatch ( e . what (), "Cannot use field() for type: int[11][10]" ));
  }

  try
  {
    cout << c . element () << endl;
    assert ( "element: missing exception!" == nullptr );
  }
  catch ( const invalid_argument & e )
  {
    assert ( whitespaceMatch ( e . what (), "Cannot use element() for type: struct\n"
    "{\n"
    "  int m_First;\n"
    "  enum\n"
    "  {\n"
    "    NEW,\n"
    "    FIXED,\n"
    "    BROKEN,\n"
    "    DEAD\n"
    "  } m_Second;\n"
    "  double m_Third;\n"
    "}" ));
  }

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
