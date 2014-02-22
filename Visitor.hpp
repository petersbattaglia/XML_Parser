#ifndef PBATTAG1_VISITOR_HPP
#define PBATTAG1_VISITOR_HPP

#include "Visitor.hpp"
#include "Element.hpp"
#include "Text.hpp"

namespace xml
{
	class Element;

	using namespace xml;
	class Visitor {
		public:
			virtual void start_element_visit(const Element &e) = 0;
			virtual void end_element_visit(const Element &e) = 0;
			virtual void visit_text(const Text &t) = 0;
	};
}


#endif
