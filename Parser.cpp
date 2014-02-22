#include "Parser.hpp"

using namespace std;

namespace xml
{
	void split(const string s, char sep, vector<string> *list, bool skipWhitespace);
	bool isStringAlphaNumericOrUnderScore(const char* str, int length);
	void die(Element* root);

	Parser::Parser() {}

	enum ParserState {
		ELEMENT = 1,
		TEXT_CONTENT,
		COMMENT,
		END_ELEMENT
	} state = ELEMENT ;

	const Element *
	Parser::parse(const char *doc, size_t sz)
	{
		bool beenToElem = false;
		Element* rootElem = NULL;
		string contents(doc);
		map<string, stack<string> > namespaces;
		int index = 0;
		string currentPart;
		stack<string> elementStack = stack<string>();
		stack< deque<string> > nsStack = stack< deque <string> >(); // Holds the ns aliases to pop when an element closes. Bound by index to above.
		stack<Element*> elementRefStack = stack<Element*>();

		// Preprocessor - Removes any newlines from element tags (start and end)
		bool isWithinTags = true;
		for(int i = 0; i < sz; i++)
		{
			if(contents[i] == '<')
				isWithinTags = true;
			else if(contents[i] == '>')
				isWithinTags = false;

			if(contents[i] == '\n')
				if(isWithinTags)
					contents[i] = ' ';
		}

		// Run-off initial whitespace
		while(isspace(contents[index])) index++;

		if(contents.size() - index < 7) {die(rootElem); return NULL;}

		while(index < contents.size())
		{	
			if(contents[index] == '<' && contents[index+1] == '!' && contents[index+2] == '-' && contents[index+3] == '-')
			{
				beenToElem = true;
				state = COMMENT; 
				int posn = index;
				while(!(contents[index] == '-' && contents[index+1] == '-' && contents[index+2] == '>') && index < contents.size()) index++;
				if(index == contents.size()) {die(rootElem); return NULL;}
				index += 3;
				currentPart = contents.substr(posn, index - posn);
			}
			else if(contents[index] == '<' && contents[index + 1] == '/')
			{
				beenToElem = true;
				state = END_ELEMENT;
				int posn = index;
				while(contents[index] != '>' && index < contents.size()) index++;
				if(index == contents.size()) {die(rootElem); return NULL;}
				currentPart = contents.substr(posn, ++index - posn);
			}
			else if(contents[index] == '<')
			{
				beenToElem = true;
				state = ELEMENT;
				int posn = index;
				while(contents[index] != '>' && index < contents.size()) index++;
				if(index == contents.size()) {die(rootElem); return NULL;}
				currentPart = contents.substr(posn, ++index - posn);
			}
			else
			{
				if(!beenToElem) {die(rootElem); return NULL;}
				// Start reading this as text. 
				state = TEXT_CONTENT;
				int posn = index;
				while(contents[index] != '<' && index < contents.size()) index++;
				if(index == contents.size()) {die(rootElem); return NULL;}
				currentPart = contents.substr(posn, index - posn);
			}

			if(state == ELEMENT)
			{	
				deque<string> currentDefinedAliases = deque<string>();
		
				// Tokenize the element start tag
				vector<string> elementTokens = vector<string>();
				split(currentPart, ' ', &elementTokens, true);

				// Fix the element name
				elementTokens[0] = elementTokens[0].substr(1, elementTokens[0].length() - 1);
		
				// Simple start tag: no declared namespace nonsense. 
				if(elementTokens[0][elementTokens[0].length() - 1] == '>')
					elementTokens[0] = elementTokens[0].substr(0, elementTokens[0].length() - 1);

				// Another verification fora different case of simple start tag -- continue only if we have some namespaces
				if(elementTokens.size() > 1 && elementTokens[1][0] != '>')
				{
					for(int i = 1; i < elementTokens.size(); i++)
					{
						// Use this case to check if last elem is just a close symbol.
						if(i == elementTokens.size() - 1 && elementTokens[i][0] == '>')
							break;

						// Verify format of the namespace, quote checking later on
						if(elementTokens[i].substr(0, 6) != "xmlns:") {die(rootElem); return NULL;}		

						elementTokens[i] = elementTokens[i].substr(6, elementTokens[i].length() - 6);
						if(elementTokens[i].length() == 0) {die(rootElem); return NULL;}

						// Last element, which may have a '>' at the end.
						if(i == elementTokens.size() - 1 && elementTokens[i][elementTokens[i].length() - 1] == '>')
							elementTokens[i] = elementTokens[i].substr(0, elementTokens[i].size() - 1); // Get rid of the extra char

						vector<string> nsBreak = vector<string>();
						split(elementTokens[i], '=', &nsBreak, false);				

						if(nsBreak.size() > 2) {die(rootElem); return NULL;}
						if(nsBreak[1][0] != '"' || nsBreak[1][nsBreak[1].length() - 1] != '"') {die(rootElem); return NULL;}
						if(nsBreak[1][1] == '"' || nsBreak[1][nsBreak[1].length() - 2] == '"') {die(rootElem); return NULL;}					
						nsBreak[1] = nsBreak[1].substr(1, nsBreak[1].size() - 2); // Clean up the URL quotes

						stringstream trimmer;
						trimmer << nsBreak[1];
						nsBreak[1].clear();
						trimmer >> nsBreak[1];

						if(nsBreak[1].length() == 0) {die(rootElem); return NULL;}
						if(nsBreak[0] == "") {die(rootElem); return NULL;}
						if(!isStringAlphaNumericOrUnderScore(nsBreak[0].c_str(), nsBreak[0].length())) {die(rootElem); return NULL;}

						if(namespaces.find(nsBreak[0]) == namespaces.end()) // New NS alias
						{
							namespaces[nsBreak[0]] = stack<string>();
							namespaces[nsBreak[0]].push(nsBreak[1]);
							currentDefinedAliases.push_back(nsBreak[0]);
						} 
						else
						{
							// Check for redefinition in current elem
							if(find(currentDefinedAliases.begin(), currentDefinedAliases.end(), nsBreak[0]) != currentDefinedAliases.end()) {die(rootElem); return NULL;} 
							namespaces[nsBreak[0]].push(nsBreak[1]);
							currentDefinedAliases.push_back(nsBreak[0]);
						}
					}
				}

				elementStack.push(elementTokens[0]);
				nsStack.push(currentDefinedAliases);

				vector<string> elemBreak = vector<string>();
				split(elementTokens[0], ':', &elemBreak, false);

				if(elemBreak.size() > 2) {die(rootElem); return NULL;}
				if(!isStringAlphaNumericOrUnderScore(elemBreak[0].c_str(), elemBreak[0].length())) {die(rootElem); return NULL;}
				if(elemBreak.size() == 2)
					if(!isStringAlphaNumericOrUnderScore(elemBreak[1].c_str(), elemBreak[1].length())) {die(rootElem); return NULL;}

				string uri;
				string name;
				if(elemBreak.size() == 2)
				{	
					name = elemBreak[1];
					if(namespaces.find(elemBreak[0]) == namespaces.end()) {die(rootElem); return NULL;}
					if(namespaces[elemBreak[0]].size() == 0) {die(rootElem); return NULL;}
					uri = namespaces[elemBreak[0]].top();
				} 
				else
				{
					uri = "";
					name = elemBreak[0];
				}

				Element* currentElement = new Element();
				currentElement->setName(name);
				currentElement->setNmspace(uri);
			
				if(rootElem == NULL)
					rootElem = currentElement;

				if(elementRefStack.size() > 0)	
					elementRefStack.top()->addChild(currentElement);

				elementRefStack.push(currentElement);
			}
			else if(state == END_ELEMENT)
			{
				if(rootElem == NULL)  {die(rootElem); return NULL;}

				currentPart = currentPart.substr(2, currentPart.size() - 3);

				if(isspace(currentPart[0])) {die(rootElem); return NULL;}
				stringstream trimmer;
				trimmer << currentPart;
				currentPart.clear();
				trimmer >> currentPart;

				vector<string> elemBreak = vector<string>();
				split(currentPart, ':', &elemBreak, false);

				if(elemBreak.size() > 2) {die(rootElem); return NULL;}

				if(!isStringAlphaNumericOrUnderScore(elemBreak[0].c_str(), elemBreak[0].length())) {die(rootElem); return NULL;}
				if(elemBreak.size() == 2)
					if(!isStringAlphaNumericOrUnderScore(elemBreak[1].c_str(), elemBreak[1].length())) {die(rootElem); return NULL;}

				if(elementStack.size() == 0) {die(rootElem); return NULL;}

				if(elementStack.top() != currentPart) {die(rootElem); return NULL;}
				elementStack.pop();

				while(nsStack.top().size() > 0)
				{
					namespaces[nsStack.top().front()].pop();
					nsStack.top().pop_front();
				}
				nsStack.pop();
			
				if(elementStack.size() == 0)
					break;

				if(elementRefStack.size() == 0) {die(rootElem); return NULL;}
				elementRefStack.pop();
			}
			else if(state == TEXT_CONTENT)
			{
				if(rootElem == NULL)  {die(rootElem); return NULL;}

				Text* textElem = new Text();
				textElem->setData(currentPart);
			
				if(elementRefStack.size() == 0) {die(rootElem); return NULL;}
				elementRefStack.top()->addChild(textElem);
			}
		}

		if(elementStack.size() > 0) {die(rootElem); return NULL;}

		while(isspace(contents[index]) && index < contents.size()) index++;
		if(index != contents.size()) {die(rootElem); return NULL;}

		return rootElem;
	}

	void split(const string s, char sep, vector<string> *list, bool skipWhitespace) {
		stringstream ss(s);
		string item;
		while(getline(ss, item, sep))
		{
			stringstream trimmer;
			trimmer << item;
			item.clear();
			trimmer >> item;
			if(skipWhitespace)
			{
				if (item != "") list->push_back(item);
			}
			else
			{
				list->push_back(item);
			}
		}
	}

	bool isStringAlphaNumericOrUnderScore(const char* str, int length)
	{
		if(length == 0) return false;
		for(int i = 0; i < length; i++)
			if(!(isalpha(str[i]) || isdigit(str[i]) || str[i] == '_'))
				return false;
		return true;
	}

	// General Error
	void die(Element* root)
	{
		cout << "Invalid XML Input." << endl;
		if(root != NULL) delete root;
	}
}
