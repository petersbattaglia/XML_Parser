#ifndef PBATTAG1_TEXT_HPP
#define PBATTAG1_TEXT_HPP

#include "String.hpp"
#include "Node.hpp"

namespace xml
{
	class Text : public Node {
	    public:
		const String &str() const;
		void setData(std::string txt);
		static bool is_Text(const Node *);
		static const Text *to_Text(const Node *);
	    private:
		String text;
	};
}

#endif
