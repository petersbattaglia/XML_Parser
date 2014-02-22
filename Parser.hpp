#ifndef PBATTAG1_PARSER_HPP
#define PBATTAG1_PARSER_HPP

#include "Element.hpp"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <deque>
#include <map>
#include <stack>
#include <algorithm>
#include <iterator>
#include <sstream>

namespace xml
{
	class Parser {
	    public:
		Parser();
		const Element* parse(const char *doc, size_t sz);
	};
}

#endif
