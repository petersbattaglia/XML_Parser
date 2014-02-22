#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <assert.h>
#include "Parser.hpp"
#include "Text.hpp"
#include "Element.hpp"
#include "Visitor.hpp"

using namespace std;
using namespace xml;

// Escape strings for clearer printing and debugging.
std::string
escape_string(const std::string &in) {

    std::string s;
    // Escaped string won't be bigger than double the original size.
    s.reserve(2*s.size());

    for (std::string::const_iterator it = in.begin(); it < in.end(); ++it) {
        switch (*it) {
            case '\n':
                s.append("\\n");
                break;
            case '\r':
                s.append("\\r");
                break;
            case '\t':
                s.append("\\t");
                break;
            case '\f':
                s.append("\\f");
                break;
            case '\v':
                s.append("\\v");
                break;
            case '"':
                s.append("\\\"");
                break;
            default:
                s.push_back(*it);
                break;
        }
    }

    return s;
}

void
traverse(const Node *n, size_t current_indent, bool indent_flag) {

    string indent(indent_flag ? string(current_indent, ' ') : "");

    if (Element::is_Element(n)) {

        const Element *e = Element::to_Element(n);
        cout << indent << "Start: " << e->nmspace() << ":" << e->name() << endl;
        for (size_t i = 0; i < e->n_children(); i++) {
            traverse(e->child(i), current_indent + 2, indent_flag);
        }
        cout << indent << "End: " << e->nmspace() << ":" << e->name() << endl;

    } else if (Text::is_Text(n)) {

        const Text *t = Text::to_Text(n);
        cout << indent <<  "Text: \"" << escape_string(t->str()) << "\"" << endl;

    } else {

        assert(false);
    }
}

class MyVisitor : public xml::Visitor {
    public:
        MyVisitor(bool ind_f) : indent(0), indent_flag(ind_f) {}
        virtual void start_element_visit(const xml::Element &);
        virtual void end_element_visit(const xml::Element &);
        virtual void visit_text(const xml::Text &);
    private:
        size_t indent;
        const bool indent_flag;
};

void
MyVisitor::start_element_visit(const xml::Element &e) {
    string is(indent_flag ? string(indent, ' ') : "");
    cout << is << "Starting visit of element " << e.nmspace() << ":"
         << e.name() << "..." << endl;
    indent += 2;
}

void
MyVisitor::end_element_visit(const xml::Element &e) {
    indent -= 2;
    string is(indent_flag ? string(indent, ' ') : "");
    cout << is << "Ending visit of element " << e.nmspace() << ":"
         << e.name() << "." << endl;
}

void
MyVisitor::visit_text(const xml::Text &t) {
    string is(indent_flag ? string(indent, ' ') : "");
    cout << is << "Visiting text content \"" << escape_string(t.str()) << "\"." << endl;
}


  string num2Str ( int Number )
  {
     ostringstream ss;
     ss << Number;
     return ss.str();
  }

int main (int argc, char *argv[]) 
{
	string filename;
	if(true)
	{
		bool indent = true;
		filename = argv[1];
		filename = "input/" + filename;
		std::ifstream in(filename.c_str(), std::ios::in);
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();

		xml::Parser parser;
		const xml::Element* x = parser.parse(contents.c_str(), contents.size());
		if(x != NULL) 
		{
			traverse(x, 0, indent);
			MyVisitor v(indent);
			x->accept(&v);
			delete x;
		}
		else { cout << "Failed" << endl << endl; }
	}
	

	return 0;
}


















