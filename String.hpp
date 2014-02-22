#ifndef PBATTAG1_STRING_HPP
#define PBATTAG1_STRING_HPP
#include "String.hpp"
#include <string>
#include <iostream>

namespace xml
{
	class String {
		public:
			String();
			String(const String &);
			operator std::string() const;
			String &operator=(const String &);
			String(std::string inStr);
		private:
			std::string strMember;
	};

	// Comparison with std::string.
	bool operator==(const std::string &x, const String &y);
	bool operator==(const String &y, const std::string &x);
	bool operator!=(const std::string &x, const String &y);
	bool operator!=(const String &y, const std::string &x);

	// Comparison with C string.
	bool operator==(const char *x, const String &y);
	bool operator==(const String &y, const char *x);
	bool operator!=(const char *x, const String &y);
	bool operator!=(const String &y, const char *x);

	std::ostream &operator<<(std::ostream &out, const String &x);
}

#endif
