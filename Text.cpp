#include "Text.hpp"

namespace xml 
{
	const String &
	Text::str() const
	{
		return text;
	}

	void 
	Text::setData(std::string txt)
	{
		text = txt;
	}

	bool 
	Text::is_Text(const Node *n)
	{
		const Text* textPtr = dynamic_cast<const Text*>(n);
		if(textPtr != NULL) return true;
		else return false;
	}

	const Text*
	Text::to_Text(const Node *n)
	{
		const Text* textPtr = dynamic_cast<const Text*>(n);
		return textPtr;
	}
}
