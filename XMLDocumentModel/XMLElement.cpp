///////////////////////////////////////////////////////////////////
// XmlElement.cpp - define XML Element types                     //
// ver 1.4                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013   //
// Author:      Pramukh Shenoy,Syracuse University               //
//              (315) 746-0822, pbantwal@syr.edu                 //
// Original     Jim Fawcett, CST 4-187, 443-3948				 //
// Author:      jfawcett@twcny.rr.com						     //
///////////////////////////////////////////////////////////////////

#include "XmlElement.h"
#include <iostream>

using namespace XMLDocumentModel;

size_t AbstractXmlElement::count = 0;
size_t AbstractXmlElement::tabSize = 2;

//----< factory for doc elements >----------------------------------------

std::shared_ptr<AbstractXmlElement> XMLDocumentModel::makeDocElement(std::shared_ptr<AbstractXmlElement> pRoot)
{
	std::shared_ptr<AbstractXmlElement> ptr(new DocElement(pRoot));
	return ptr;
}

//----< factory for tagged elements >----------------------------------------

std::shared_ptr<AbstractXmlElement> XMLDocumentModel::makeTaggedElement(const std::string& tag)
{
	std::shared_ptr<AbstractXmlElement> ptr(new TaggedElement(tag));
	return ptr;
}
//----< factory for text elements >------------------------------------------

std::shared_ptr<AbstractXmlElement> XMLDocumentModel::makeTextElement(const std::string& text)
{
	std::shared_ptr<AbstractXmlElement> ptr(new TextElement(text));
	return ptr;
}
//----< factory for comment elements >---------------------------------------

std::shared_ptr<AbstractXmlElement> XMLDocumentModel::makeCommentElement(const std::string& text)
{
	std::shared_ptr<AbstractXmlElement> ptr(new CommentElement(text));
	return ptr;
}
//----< factory for processing instruction elements >------------------------

std::shared_ptr<AbstractXmlElement> XMLDocumentModel::makeProcInstrElement(const std::string& text)
{
	std::shared_ptr<AbstractXmlElement> ptr(new ProcInstrElement(text));
	return ptr;
}
//----< factory for XML Declaration elements >-------------------------------

std::shared_ptr<AbstractXmlElement> XMLDocumentModel::makeXmlDeclarElement()
{
	std::shared_ptr<AbstractXmlElement> ptr(new XmlDeclarElement());
	return ptr;
}
//----< doc element constructor and destructor >-----------------------------

DocElement::DocElement(std::shared_ptr<AbstractXmlElement> pRoot)
{
	if (pRoot == nullptr)
	{
		std::shared_ptr<AbstractXmlElement> pDecl = makeXmlDeclarElement();
		pDecl->addAttrib("version", "1.0");
		children_.push_back(pDecl);
	}
	else
		children_.push_back(pRoot);
}
//----< private helper for DocElement::addChild(...) >-----------------------

bool DocElement::hasXmlRoot()
{
	for (auto pElement : children_)
	{
		if (dynamic_cast<TaggedElement*>(pElement.get()) != nullptr)
			return true;
	}
	return false;
}
//----< add only one child to doc element using pointer to child >-----------

bool DocElement::addChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	// don't add TextElements
	TextElement* pTxEl = dynamic_cast<TextElement*>(pChild.get());
	if (pTxEl != 0)  // don't add text elements to DocElement
		return false;

	// don't add another DocElement
	DocElement* pDcEl = dynamic_cast<DocElement*>(pChild.get());
	if (pDcEl != 0)  // don't add text elements to DocElement
		return false;

	// add ProcInstrElements and CommentElements 
	TaggedElement* te = dynamic_cast<TaggedElement*>(pChild.get());
	if (te == nullptr) // is not a TaggedElement
	{
		children_.push_back(pChild);
		return true;
	}

	// add only one TaggedElement
	if (!hasXmlRoot())
	{
		children_.push_back(pChild);
		return true;
	}
	return false;
}
//----< remove child from doc element using pointer to child >---------------

bool DocElement::removeChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	auto iter = std::find(begin(children_), end(children_), pChild);
	if (iter != end(children_))
	{
		children_.erase(iter);
		return true;
	}
	return false;
}
//-----< return the list of children >---------------------------------

std::vector<std::shared_ptr<AbstractXmlElement>> DocElement::children()
{
	return children_;
}
//----< return value = tag from doc element >-----------------------------

std::string DocElement::value() { return std::string(""); }

//----< generate xml string for tagged element >-----------------------------

std::string DocElement::toString()
{
	std::string rtn;
	for (auto elem : children_)
		rtn += elem->toString();
	return rtn;
}
//----< add child to tagged element using pointer to child >-----------------

bool TaggedElement::addChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	children_.push_back(pChild);
	return true;
}
//----< remove child from tagged element using pointer to child >------------

bool TaggedElement::removeChild(std::shared_ptr<AbstractXmlElement> pChild)
{
	auto iter = std::find(begin(children_), end(children_), pChild);
	if (iter != end(children_))
	{
		children_.erase(iter);
		return true;
	}
	return false;
}
//----< remove child from tagged element using child's value >---------------

bool TaggedElement::removeChild(const std::string& value)
{
	for (auto iter = begin(children_); iter != end(children_); ++iter)
	{
		if ((*iter)->value() == value)
		{
			children_.erase(iter);
			return true;
		}
	}
	return false;
}
//----< add attribute to tagged element >------------------------------------

bool TaggedElement::addAttrib(const std::string& name, const std::string& value)
{
	std::pair<std::string, std::string> atPair(name, value);
	attribs_.push_back(atPair);
	return true;
}
//----< remove attribute from tagged element >-------------------------------

bool TaggedElement::removeAttrib(const std::string& name)
{
	for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
	{
		if (iter->first == name)
		{
			attribs_.erase(iter);
			return true;
		}
	}
	return false;
}
//----< return value = tag from tagged element >-----------------------------

std::string TaggedElement::value() { return tag_; }

//----< generate xml string for tagged element >-----------------------------

std::string TaggedElement::toString()
{
	std::string spacer(tabSize*++count, ' ');
	std::string xml = "\n" + spacer + "<" + tag_;
	for (auto at : attribs_)
	{
		xml += " ";
		xml += at.first;
		xml += "=\"";
		xml += at.second;
		xml += "\"";
	}
	xml += ">";
	for (auto pChild : children_)
		xml += pChild->toString();
	xml += "\n" + spacer + "</" + tag_ + ">";
	--count;
	return xml;
}
//-----< return the list of children >---------------------------------

std::vector<std::shared_ptr<AbstractXmlElement>> TaggedElement::children()
{
	return children_;
}
//-----< return the list of attributes >---------------------------------

std::vector<std::pair<std::string, std::string>> TaggedElement::attributes()
{
	return attribs_;
}
//----< generate xml string for text element >-------------------------------

std::string TextElement::toString()
{
	std::string spacer(tabSize * ++count, ' ');
	std::string xml = "\n" + spacer + text_;
	--count;
	return xml;
}
//----< add attribute to ProcInstElement >-----------------------------------

bool ProcInstrElement::addAttrib(const std::string& name, const std::string& value)
{
	std::pair<std::string, std::string> atPair(name, value);
	attribs_.push_back(atPair);
	return true;
}
//----< remove attribute from ProcInstElement >------------------------------

bool ProcInstrElement::removeAttrib(const std::string& name)
{
	for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
	{
		if (iter->first == name)
		{
			attribs_.erase(iter);
			return true;
		}
	}
	return false;
}
//-----< return the list of attributes >---------------------------------

std::vector<std::pair<std::string, std::string>> ProcInstrElement::attributes()
{
	return attribs_;
}
//----< generate xml string for text element >-------------------------------

std::string XmlDeclarElement::toString()
{
	std::string spacer(tabSize * ++count, ' ');
	std::string xml = "\n" + spacer + "<?xml";
	for (auto at : attribs_)
	{
		xml += " ";
		xml += at.first;
		xml += "=\"";
		xml += at.second;
		xml += "\"";
	}
	xml += ">";
	--count;
	return xml;
}
//----< add attribute to ProcInstElement >-----------------------------------

bool XmlDeclarElement::addAttrib(const std::string& name, const std::string& value)
{
	std::pair<std::string, std::string> atPair(name, value);
	attribs_.push_back(atPair);
	return true;
}
//----< remove attribute from ProcInstElement >------------------------------

bool XmlDeclarElement::removeAttrib(const std::string& name)
{
	for (auto iter = begin(attribs_); iter != end(attribs_); ++iter)
	{
		if (iter->first == name)
		{
			attribs_.erase(iter);
			return true;
		}
	}
	return false;
}
//----< generate xml string for comment element >-----------------------------

std::string CommentElement::toString()
{
	std::string spacer(tabSize*++count, ' ');
	std::string xml = "\n" + commentText_;
	--count;
	return xml;
}

//----< generate xml string for proc element >-----------------------------

std::string ProcInstrElement::toString()
{
	std::string spacer(tabSize*++count, ' ');
	std::string xml = "\n" + spacer + "<?" + text;
	for (auto at : attribs_)
	{
		xml += " ";
		xml += at.first;
		xml += "=\"";
		xml += at.second;
		xml += "\"";
	}
	xml += "?>";
	--count;
	return xml;
}

//-----< return the list of attributes >---------------------------------

std::vector<std::pair<std::string, std::string>> XmlDeclarElement::attributes()
{
	return attribs_;
}
//----< helper function displays titles >------------------------------------

void XMLDocumentModel::title(const std::string& title, char underlineChar)
{
	std::cout << "\n\n  " << title;
	std::cout << "\n " << std::string(title.size() + 2, underlineChar);
	std::cout << "\n";
}

//----< test stub >----------------------------------------------------------

#ifdef TEST_XMLELEMENT

int main()
{
	title("Testing XmlElement Package", '=');
	std::cout << "\n";

	using sPtr = std::shared_ptr < AbstractXmlElement >;

	sPtr root = makeTaggedElement("root");
	root->addChild(makeTextElement("this is a test"));

	sPtr child = makeTaggedElement("child");
	child->addChild(makeTextElement("this is another test"));
	child->addAttrib("first", "test");
	root->addChild(child);

	sPtr docEl = makeDocElement(root);
	std::cout << "  " << docEl->toString();
	std::cout << "\n\n";
}

#endif
