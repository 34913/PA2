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

	CInterface(int id)
	:	id(id)
	{}

	virtual size_t getSize() const  = 0;

	virtual bool operator==( const CInterface & x ) const
    {
		return this->id == x.id;
    }
	
	bool operator!=( const CInterface & x ) const
	{
		return !( *this == x );
	}

	virtual void print( ostream & os ) const = 0;

	friend ostream & operator << ( ostream & os, const CInterface & x )
	{
		x.print( os );
		return os;
	}
 
	virtual CInterface * clone() const = 0;

	virtual ~CInterface()
	{}

};

class CDataTypeInt: public CInterface
{
public:

	CDataTypeInt()
	:	CInterface(1)
	{}

	size_t getSize() const override
	{
		return 4;
	}

    void print( ostream & os ) const override
    {
        os << "int";
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
	:	CInterface(2)
	{}

    size_t getSize() const override
    {
        return 8;
    }

	void print( ostream & os) const override
    {
        os << "double";
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
	:	CInterface(3)
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
		os << "enum" << endl << "{" << endl;
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
	:	CInterface(4)
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
				string str = "Duplicate field: ";
				str += name;
				throw invalid_argument( str );
			}
		}

		shared_ptr<CInterface> copy( type.clone() );
		content.push_back( { name, copy } );

		return *this;
	}

	CInterface & field( const string & name ) const
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
		os << "struct" << endl << "{" << endl;

		for( size_t i = 0; i < content.size(); i++ )
			os << *content[ i ].second << " " << content[ i ].first << ";" << endl;
		
		os << "}";
	}

	CDataTypeStruct * clone() const override
	{
		return new CDataTypeStruct( *this );
	}

};

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

	CDataTypeStruct a = CDataTypeStruct()
		.addField("m_Length", CDataTypeInt())
		.addField("m_Status", CDataTypeEnum()
			.add("NEW")
			.add("FIXED")
			.add("BROKEN")
			.add("DEAD"))
		.addField("m_Ratio", CDataTypeDouble());

	CDataTypeStruct b = CDataTypeStruct().addField("m_Length", CDataTypeInt()).addField("m_Status", CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("READY")).addField("m_Ratio", CDataTypeDouble());

	CDataTypeStruct c = CDataTypeStruct().addField("m_First", CDataTypeInt()).addField("m_Second", CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("DEAD")).addField("m_Third", CDataTypeDouble());

	CDataTypeStruct d = CDataTypeStruct().addField("m_Length", CDataTypeInt()).addField("m_Status", CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("DEAD")).addField("m_Ratio", CDataTypeInt());

	assert(whitespaceMatch(a, "struct\n"
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
							  "}"));

	assert(whitespaceMatch(b, "struct\n"
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
							  "}"));

	assert(whitespaceMatch(c, "struct\n"
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
							  "}"));

	assert(whitespaceMatch(d, "struct\n"
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
							  "}"));


	assert(a == c);
	assert(a != d);
	assert(a != b);
	assert(a.field("m_Status") == CDataTypeEnum().add("NEW").add("FIXED").add("BROKEN").add("DEAD"));
	assert(a.field("m_Status") != CDataTypeEnum().add("NEW").add("BROKEN").add("FIXED").add("DEAD"));
	assert(a != CDataTypeInt());
	assert(whitespaceMatch(a.field("m_Status"), "enum\n"
												"{\n"
												"  NEW,\n"
												"  FIXED,\n"
												"  BROKEN,\n"
												"  DEAD\n"
												"}"));

	CDataTypeStruct aOld = a;
	b.addField("m_Other", CDataTypeDouble());

	a.addField("m_Sum", CDataTypeInt());

	assert(a != aOld);
	assert(a != c);
	assert(aOld == c);

	assert(whitespaceMatch(a, "struct\n"
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
							  "}"));

	assert(whitespaceMatch(b, "struct\n"
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
							  "  double m_Other;\n"
							  "}"));

	c.addField("m_Another", a.field("m_Status"));

	assert(whitespaceMatch(c, "struct\n"
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
							  "  enum\n"
							  "  {\n"
							  "    NEW,\n"
							  "    FIXED,\n"
							  "    BROKEN,\n"
							  "    DEAD\n"
							  "  } m_Another;\n"
							  "}"));

	d.addField("m_Another", a.field("m_Ratio"));

	assert(whitespaceMatch(d, "struct\n"
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
							  "  double m_Another;\n"
							  "}"));

	assert(a.getSize() == 20);
	assert(b.getSize() == 24);

	try
	{
		a.addField("m_Status", CDataTypeInt());
		assert("addField: missing exception!" == nullptr);
	}
	catch (const invalid_argument &e)
	{
		assert(e.what() == "Duplicate field: m_Status"s);
	}

	try
	{
		cout << a.field("m_Fail") << endl;
		assert("field: missing exception!" == nullptr);
	}
	catch (const invalid_argument &e)
	{
		assert(e.what() == "Unknown field: m_Fail"s);
	}

	try
	{
		CDataTypeEnum en;
		en.add("FIRST").add("SECOND").add("FIRST");
		assert("add: missing exception!" == nullptr);
	}
	catch (const invalid_argument &e)
	{
		assert(e.what() == "Duplicate enum value: FIRST"s);
	}

	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
