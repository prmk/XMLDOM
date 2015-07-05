/////////////////////////////////////////////////////////////////////////////
// XmlDocument.h - a container of XmlElement nodes						   //
//																		   //
// Ver 2.5																   //
// ----------------------------------------------------------------------- //
// copyright © Pramukh Shenoy, 2015                                        //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Yoga Pro 2												   //
// Application: OOD Project, 2015	                                       //
// Author:      Pramukh Shenoy,Syracuse University                         //
//              (315) 746-0822, pbantwal@syr.edu                           //
// Original     Jim Fawcett, CST 4-187, 443-3948						   //
// Author:      jfawcett@twcny.rr.com									   //
/////////////////////////////////////////////////////////////////////////////

#include "XmlDocument.h"
#include <iostream>
#include <string>
#include <algorithm>

using namespace XMLDocumentModel;
using namespace std;

//constructor of XMlDocument
//reads the source: file name in case of a file and contents in case of a string
//Calls the TOker to tokenize the data
//XmlParts creates a XMLized token of the input data
//for each of the data, XMLElement is used to create an element based on the data
//finally constructs an XML document with the prologue, document root and the epilog
XMLDocumentModel::XmlDocument::XmlDocument(const std::string& src, bool isFile)
{
	try
	{
		Toker toker(src, isFile);
		toker.setMode(Toker::xml);
		XmlParts parts(&toker);
		if (src.length() > 0)
		{
			while (parts.get())
			{
				currTok = parts.show();
				makeElement();
			}
		}

		pDocElement_ = makeDocElement(declElem);
		if (pDocElement_ != nullptr && procElem != nullptr)
			pDocElement_->addChild(procElem);
		if (pDocElement_ != nullptr && commElem != nullptr)
			pDocElement_->addChild(commElem);
		if (pDocElement_ != nullptr && root != nullptr)
			pDocElement_->addChild(root);

	}
	catch (std::exception ex)
	{
		std::cout << "\n  " << ex.what() << "\n\n";
	}
}

//----<move constructor >--------------------------------------
XmlDocument::XmlDocument(XmlDocument&& XmlDoc) : pDocElement_(XmlDoc.pDocElement_)
, root(XmlDoc.root), procElem(XmlDoc.procElem), commElem(XmlDoc.commElem), declElem(XmlDoc.declElem)
{
	XmlDoc.pDocElement_ = nullptr;
	XmlDoc.root = nullptr;
	XmlDoc.procElem = nullptr;
	XmlDoc.commElem = nullptr;
	XmlDoc.declElem = nullptr;
};

//move assignment operator
XmlDocument& XmlDocument::operator=(XmlDocument&& XmlDom) {

	if (this == &XmlDom) return *this;

	pDocElement_ = XmlDom.pDocElement_;
	root = XmlDom.root;
	procElem = XmlDom.procElem;
	commElem = XmlDom.commElem;
	declElem = XmlDom.declElem;

	XmlDom.pDocElement_ = nullptr;
	XmlDom.root = nullptr;
	XmlDom.procElem = nullptr;
	XmlDom.commElem = nullptr;
	XmlDom.declElem = nullptr;

	return *this;
}

//finds out the type of the input token
//checks for Processing Element, Declare Element, Comment Element
//Tagged Element and Text Element
//creates a child element based on the type
//tags the child element to its appropriate parent
void XmlDocument::makeElement()
{
	bool taggedElemCreateChild = false, textElem = false;
	if (isNotTextElem())
	{
		if (isDeclareElem())
			makeDocElementChild();
		else if (isProcElem())
			makeProcChild();
		else if (isCommentElem())
			commElem = makeCommentElement(currTok);
		else
			taggedElemCreateChild = makeTaggedElemChild();
	}
	else
	{
		child = makeTextElement(currTok);
		textElem = true;
	}

	if (taggedElemCreateChild || textElem)
	{
		if (root == nullptr)
			root = child;
		else if (!tagElements.empty())
			(tagElements.top())->addChild(child);
		else
			root->addChild(child);
		if (!textElem)
			tagElements.push(child);
	}
}

//checks whether a XMLToken is a text element
//and returns a bool denoting the same
bool XmlDocument::isNotTextElem()
{
	return (currTok.find("<") != string::npos && currTok.find(">") != string::npos);
}

//if the element is a declare element, this function is called
//creates a declare element and assigns to declElem
//checks for attributes and adds them to the child
void XmlDocument::makeDocElementChild()
{
	declElem = makeXmlDeclarElement();
	if (hasAttribute() && declElem != nullptr)
	{
		attribElems attribs = attributes(currTok);
		for (size_t i = 0; i < attribs.size(); ++i)
			declElem->addAttrib(attribs[i].first.c_str(), attribs[i].second.c_str());
	}
}

//if the element is a proc element, this function is called
//creates a proc element and assigns to declElem
//checks for attributes and adds them to the child
void XmlDocument::makeProcChild()
{
	std::string tag = extractTag();
	procElem = makeProcInstrElement(tag);
	if (hasAttribute() && procElem != nullptr)
	{
		attribElems attribs = attributes(currTok);
		for (size_t i = 0; i < attribs.size(); ++i)
			procElem->addAttrib(attribs[i].first.c_str(), attribs[i].second.c_str());
	}
}


//if the element is a tagged element, this function is called
//creates a child element and returns true indicating the child has to be added to the parent
//checks for self closing tagged element; in which case, it creates a tagged element
//assigns to the parent and returns false, denoting the child has already been added to the parent
//checks for closing tag braces, pops the stack; returns false, denoting there is nothing to do
//checks for attributes and adds them to the child
bool XmlDocument::makeTaggedElemChild()
{
	if (isTagCloseElem())
	{
		if (!tagElements.empty())
			tagElements.pop();
		return false;
	}
	else if (isTagSelfCloseElem())
	{
		std::string tag = extractTag();
		child = makeTaggedElement(tag);
		if (hasAttribute())
		{
			attribElems attribs = attributes(currTok);
			for (size_t i = 0; i < attribs.size(); ++i)
				child->addAttrib(attribs[i].first.c_str(), attribs[i].second.c_str());
		}
		if (!tagElements.empty())
			(tagElements.top())->addChild(child);
		return false;
	}
	else
	{
		std::string tag = extractTag();
		child = makeTaggedElement(tag);
		if (hasAttribute())
		{
			attribElems attribs = attributes(currTok);
			for (size_t i = 0; i < attribs.size(); ++i)
				child->addAttrib(attribs[i].first.c_str(), attribs[i].second.c_str());
		}
	}

	return true;
}

//checks if the XMLToken is of type Declare Element
bool XmlDocument::isDeclareElem()
{
	if (currTok.find("?") != string::npos && currTok.find("xml") != string::npos && currTok.find("version") != string::npos)
		return true;
	return false;
}

//checks if the XMLToken is of type Processing Instruction Element
bool XmlDocument::isProcElem()
{
	if (currTok.find("?") != string::npos && currTok.find("xml") != string::npos)
		return true;
	return false;
}

//checks if the XMLToken is of type Comment Element
bool XmlDocument::isCommentElem()
{
	if (currTok.find("!") != string::npos && currTok.find("--") != string::npos)
		return true;
	return false;
}

//checks if the XMLToken is of type Closing Tagged Element 
bool XmlDocument::isTagCloseElem()
{
	if (currTok.find("</") != string::npos || currTok.find("< /") != string::npos)
		return true;
	return false;
}

//checks if the XMLToken is a Self Closing Tagged Element
bool XmlDocument::isTagSelfCloseElem()
{
	if (currTok.find("/>") != string::npos || currTok.find("/ >") != string::npos)
		return true;
	return false;
}

//checks of the XMLToken has attributes
bool XmlDocument::hasAttribute()
{
	if (currTok.find("=") != string::npos && (currTok.find("'") != string::npos || currTok.find("\"") != string::npos))
		return true;
	return false;
}

//check for XML Special Characters
bool isSpecialChar(char ch)
{
	bool test = isspace(ch) || ch == '/' || ch == '>' || ch == '<' || ch == '=';
	test = test || ch == '\'' || ch == '"' || ch == '--' || ch == '!' || ch == '?';
	return test;
}

//extracts the tag name from the XMlToken
//uses isSpecialChar to check for special characters
std::string XmlDocument::extractTag()
{
	std::string ident;
	size_t pos = 0;
	while (true)
	{
		char ch = currTok[pos];
		if (isSpecialChar(ch))
			++pos;
		else
			break;
	}

	while (pos < currTok.length())
	{
		char ch = currTok[pos];
		if (isSpecialChar(ch))
			break;
		ident += ch;
		++pos;
	}
	return ident;
}

//returns an instance of the XML DOM
const XmlDocument::sPtr XmlDocument::getDOM()
{
	return pDocElement_;
}

//extracts identifier at a particular position of an XMLDOM
//same as extractTag() except that the position persists across calls
std::string XmlDocument::extractIdentifier(const string & str, size_t& pos)
{
	string ident;
	while (true)
	{
		char ch = str[pos];
		if (isSpecialChar(ch))
			++pos;
		else
			break;
	}

	while (pos < str.length())
	{
		char ch = str[pos];
		if (isSpecialChar(ch))
			break;
		ident += ch;
		++pos;
	}
	return ident;
}

//searches for the list of attributes for a given XMLDOM
//stores the results in _attributes 
XmlDocument::attribElems XmlDocument::attributes(const string & str)
{
	size_t pos = 0;
	string ident = extractIdentifier(str, pos);
	_attributes.clear();
	if (ident.compare("?xml") == 0 || ident.compare("!--") == 0)
		return _attributes;

	// find attributes
	size_t locpos = str.find('>', pos);
	while (true)
	{
		std::string name = extractIdentifier(str, pos);
		if (locpos < pos)
			return _attributes;
		std::string value = extractIdentifier(str, pos);
		if (locpos < pos)
			throw std::exception("malformed XML");
		std::pair<std::string, std::string> elem;
		elem.first = name;
		elem.second = value;
		_attributes.push_back(elem);
	}

	return _attributes;
}


//this function does a depth first left to right traversal on the tree
//it prints the data as when it encounters
void XmlDocument::DFS_r_ltr(sPtr pNode)
{
	std::cout << "\n  " << pNode->value();
	for (auto pChild : pNode->children())
		DFS_r_ltr(pChild);
}

//searches for an element from the pNode with tag
//searches for the first occurance of the tag
//stores the result in found_ data
XmlDocument& XmlDocument::element(sPtr pNode, const std::string& tag)
{
	if (found_.empty() && pNode != nullptr)
	{
		for (auto pChild : pNode->children())
		{
			if (pChild->value() == tag)
			{
				found_.push_back(pChild);
				break;
			}
			element(pChild, tag);
		}
	}

	return *this;
}

//searches for a elements from the pNode with tag
//searches all the occurances of the tag
//stores the result in found_
XmlDocument& XmlDocument::elements(sPtr pNode, const std::string& tag)
{
	if (pNode != nullptr)
	{
		for (auto pChild : pNode->children())
		{
			if (pChild->value() == tag)
				found_.push_back(pChild);
			elements(pChild, tag);
		}
	}
	return *this;
}

//searches the tree starting from pNode by the attribute name
//returns the calling object to support chaining
//stores the result in found_
XmlDocument& XmlDocument::attributeByName(sPtr pNode, const std::string& attributeid)
{
	if (pNode != nullptr)
	{
		for (auto pChild : pNode->children())
		{
			for (auto pAttri : pChild->attributes())
				if (pAttri.first == attributeid)
					found_.push_back(pChild);
			attributeByName(pChild, attributeid);
		}
	}
	return *this;
}

//returns the list of attributes of all the elements in found_
//if found_ contains no elements it returns null
XmlDocument& XmlDocument::attributesList()
{
	_attributes.clear();
	if (!found_.empty())
	{
		for each (sPtr var in found_)
		{
			for (auto pAttri : var->attributes())
			{
				std::pair<std::string, std::string> elem;
				elem.first = pAttri.first;
				elem.second = pAttri.second;
				_attributes.push_back(elem);
			}
		}
	}
	return *this;
}

//adds an attribute with name and value to the elements in found_
//if there are multiple elements in found_, the attribute is added to every occurance
bool XmlDocument::addAttribute(const std::string & name, const std::string & value)
{
	if (!found_.empty())
		for each (sPtr var in found_)
			return var->addAttrib(name, value);

	return false;
}

//removes an attribute with name from the elements in found_
//if there are multiple elements in found_, the attribute is removed from every occurance
bool XmlDocument::removeAttribute(const std::string & attributeID)
{
	if (!found_.empty())
		for each (sPtr var in found_)
			for (auto pAttri : var->attributes())
				if (pAttri.first == attributeID)
					return var->removeAttrib(attributeID);

	return false;
}

//for a given tag, searches for all the children
//stores the result in found_
//if no tag is provided, takes the data from the found_
//and returns the children of found_[0]
XmlDocument& XmlDocument::children(const std::string& tag)
{
	if (tag.length() > 0)
	{
		setFoundNull();
		element(pDocElement_, tag);
	}
	if (!found_.empty())
		found_ = found_[0]->children();
	return *this;
}

//adds the child elem to all the elements in found_
//returns true if the child has been added
//returns false, otherwise.
//to use this function, syntax-
//dom.element("tag").addChild(elem)
bool XmlDocument::addChild(sPtr elem)
{
	if (!found_.empty())
		for each (sPtr var in found_)
			return var->addChild(elem);
	return false;
}

//removes the child with tag from the element in found_[0]
//returns true if the child has been removed
//returns false, otherwise.
//to use this function, syntax-
//dom.element("tag").removeChild("childtag")
bool XmlDocument::removeChild(const std::string & tag)
{
	if (!found_.empty())
	{
		sPtr parent = found_[0];
		children();
		if (!found_.empty())
		{
			for each (sPtr var in found_)
			{
				string str = var->value();
				if (str == tag)
					return parent->removeChild(var);
			}
		}
	}
	return false;
}

//returns the contents of the query results stored in found_
std::vector<XmlDocument::sPtr>& XmlDocument::select()
{
	return std::move(found_);
}

//returns the contents of the query for attribute search
XmlDocument::attribElems XmlDocument::selectAttributes()
{
	return _attributes;
}

//sets the contens of the query result to null
void XmlDocument::setFoundNull()
{
	found_.clear();
}

//sets the contens of the query result of atributes to null
void XmlDocument::setFoundAttributesNull()
{
	_attributes.clear();
}


//----< test stub >----------------------------------------------------------

#ifdef TEST_XMLDOCUMENT

int main(int argc, char* argv[])
{
	title("Testing XmlDocument class");
	cout << endl;

	for (int i = 1; i < argc; ++i)
	{
		std::cout << "\n  Processing file " << argv[i];
		std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-') << "\n\n";
		try
		{
			XmlDocument doc(argv[i], true);
			cout << doc.getDOM()->toString() << endl;
		}
		catch (std::exception ex)
		{
			std::cout << "\n  " << ex.what() << "\n\n";
		}
	}
	if (argc < 1)
	{
		std::string Prologue = "<?xml version=\"1.0\"?><!-- top level comment -->";
		std::string test1 = Prologue;
		test1 += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";
		std::string test2 = Prologue;
		test2 += "<Dir><path>/TestFile/Dir1</path><File date='notCurrent'><name type='file'>text1.txt</name><date>02/11/2012</date></File></Dir>";
		XmlDocument doc(test2, false);

		std::cout << "\n  source = " << test2.c_str() << std::endl;

		std::cout << "\n\n";

		cout << doc.getDOM()->toString() << endl;
	}
}

#endif