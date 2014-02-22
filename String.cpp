#include "String.hpp"

using namespace std;

namespace xml
{
	String::String() { }

	String::String(const String & orig)
	{
		strMember = (string)orig;
	}

	String::String(string inStr)
	{
		strMember = inStr;
	}

	String::operator string() const
	{
		return strMember;
	}

	String & String::operator=(const String &orig)
	{
		strMember = (string)orig;
		return *this;
	}

	// Comparison with std::string.
	bool operator==(const string &x, const String &y)
	{
		return x == (string)y;
	}
	bool operator==(const String &y, const string &x)
	{
		return x == (string)y;
	}
	bool operator!=(const string &x, const String &y)
	{
		return x != (string)y;
	}
	bool operator!=(const String &y, const string &x)
	{
		return x != (string)y;
	}

	// Comparison with C string.
	bool operator==(const char *x, const String &y)
	{
		string temp = x;
		return x == (string)y;
	}

	bool operator==(const String &y, const char *x)
	{
		string temp = x;
		return x == (string)y;
	}

	bool operator!=(const char *x, const String &y)
	{	
		string temp = x;
		return x != (string)y;
	}

	bool operator!=(const String &y, const char *x)
	{
		string temp = x;
		return x != (string)y;
	}

	// Output operator
	std::ostream &operator<<(std::ostream &out, const String &x)
	{
		out << (string)x;
		return out;
	}
}

