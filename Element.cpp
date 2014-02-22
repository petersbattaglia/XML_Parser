#include "Element.hpp"

namespace xml
{
	const String &
	Element::name() const
	{
		return elemName;
	}

	const String &
	Element::nmspace() const
	{
		return elemNmspace;
	}

	size_t 
	Element::n_children() const
	{
		return children.size();
	}

	const Node* 
	Element::child(size_t i) const
	{
		return children.at(i);
	}

	void 
	Element::accept(Visitor * v) const
	{
		const Element* thisElem = this;
		v->start_element_visit(*thisElem);
		for(int i = 0; i < n_children(); i++)
		{
			if(Text::is_Text(child(i)))
			{
				const Text* temp = Text::to_Text(child(i));
				v->visit_text(*temp);
			}
			else
			{
				const Element* temp = Element::to_Element(child(i));
				temp->accept(v);
			}
		
		}
		v->end_element_visit(*thisElem);
	}
		
	bool 
	Element::is_Element(const Node * n)
	{
		const Element* elemPtr = dynamic_cast<const Element*>(n);
		if(elemPtr != NULL) return true;
		else return false;
	}

	const Element*
	Element::to_Element(const Node * n)
	{
		const Element* elemPtr = dynamic_cast<const Element*>(n);
		return elemPtr;
	}

	void
	Element::setName(String name)
	{
		elemName = name;
	}

	void
	Element::setNmspace(String nmSpace)
	{
		elemNmspace = nmSpace;
	}

	void
	Element::addChild(Node *child)
	{
		children.push_back(child);
	}

	Element::~Element()
	{
		while(children.size() > 0)
		{
			delete children.back();
			children.pop_back();
		}
	}
}
