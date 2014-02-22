#ifndef PBATTAG1_ELEMENT_HPP
#define PBATTAG1_ELEMENT_HPP
#include <vector>
#include "Element.hpp"
#include "Visitor.hpp"
#include "String.hpp"
#include "Node.hpp"

class Visitor;

namespace xml
{
	class Element : public Node {
	    public:
		const String &name() const;
		const String &nmspace() const;
		size_t n_children() const;
		const Node *child(size_t i) const;
		void accept(Visitor *) const;
		static bool is_Element(const Node *);
		static const Element *to_Element(const Node *);
		void setName(String name);
		void setNmspace(String nmSpace);
		void addChild(Node* child);
		~Element();
	    private:
		std::vector<Node*> children;
		String elemName;
		String elemNmspace;
	};
}

#endif
