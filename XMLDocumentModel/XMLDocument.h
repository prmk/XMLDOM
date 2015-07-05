#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H
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
/*
* Package Operations:
* -------------------
* This package uses C++11 constructs most noteably std::shared_ptr.  
* The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees, defined in this package, are unordered trees with
* two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in the companion package, XmlElement, will be used in
* the AST defined in this package.  They are:
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration element with attributes
*
* Public Interface
* ----------------
* XMLDocument dom(src, isFile);
* XmlDocument(xmlDom) = delete;
* xmlDoc = MoveFunction();
* XmlDocument& operator=(xmlDom) = delete;
* xmlDom = MoveOperatorFunction();
* xmlDom.element(pNode,tag);
* xmlDom.elements(pNode,tag);
* xmlDom.children(tag);
* xmlDom.descendents(tag );
* xmlDom.attributeByName(pNode,attributeid);
* xmlDom.attributesList();
* xmlDom.addAttribute(tag, name);
* xmlDom.removeAttribute(attributeID);
* xmlDom.addChild(elem);
* xmlDom.removeChild(tag);
* xmlDom.select();
* xmlDom.selectAttributes();
* xmlDom.DFS_r_ltr(pNode);
* xmlDom.getDOM();
* xmlDom.setFoundNull();
* xmlDom.setFoundAttributesNull();
*
*
* Required Files:
* ---------------
*   - XmlDocument.h, XmlDocument.cpp,
*     XmlElement.h, XmlElement.cpp
*
* Build Process:
* --------------
*   devenv AST.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 2.5:
* -	Code for search functions
* - Code for children and descendents
* - Code for fetching the elements by name, id
* - Code for adding and removing a child
* - Code for adding and removing an attribute
* ver 2.0:
* -	build constructor to create a parse tree structure
* -	Move constructor
* -	Move Assignment Operator
* ver 1.2 : 21 Feb 15
* - modified these comments
* ver 1.1 : 14 Feb 15
* - minor changes to comments, method arguments
* Ver 1.0 : 11 Feb 15
* - first release
*/

#include <memory>
#include <string>
#include <iostream>
#include <stack>
#include <utility>
#include "XMLElement.h"
#include "XMLElementParts.h"

using namespace std;

namespace XMLDocumentModel
{
	class XmlDocument
	{
	public:
		using sPtr = std::shared_ptr < AbstractXmlElement > ;
		typedef std::vector<std::pair<std::string, std::string>> attribElems;

		XmlDocument(const std::string& src, bool isFile = false);
		XmlDocument(const XmlDocument& xmlDom) = delete;
		XmlDocument(XmlDocument&& xmlDoc);
		XmlDocument& operator=(const XmlDocument& xmlDom) = delete;
		XmlDocument& operator=(XmlDocument&& xmlDom);

		// queries return XmlDocument references so they can be chained, e.g., doc.element("foobar").descendents();

		XmlDocument& element(sPtr pNode, const std::string& tag);           // found_[0] contains first element (DFS order) with tag
		XmlDocument& elements(sPtr pNode, const std::string& tag);          // found_ contains all elements with tag
		XmlDocument& children(const std::string& tag = "");     // found_ contains sPtrs to children of prior found_[0] 
		XmlDocument& descendents(const std::string& tag = "");  // found_ contains sPtrs to descendents of prior found_[0]
		XmlDocument& attributeByName(sPtr pNode, const std::string& attributeid);
		XmlDocument& attributesList();
		bool addAttribute(const std::string & tag, const std::string & name);
		bool removeAttribute(const std::string & attributeID);
		bool addChild(sPtr elem);
		bool removeChild(const std::string & tag);
		std::vector<sPtr>& select();                            // return reference to found_.  Use std::move(found_) to clear found_
		attribElems selectAttributes();
		void DFS_r_ltr(sPtr pNode);
		const sPtr getDOM();
		void setFoundNull();
		void setFoundAttributesNull();

	private:
		sPtr pDocElement_;         // AST that holds procInstr, comments, XML root, and more comments
		std::vector<sPtr> found_;  // query results
		std::string currTok;
		sPtr root;
		sPtr child;
		sPtr procElem;
		sPtr commElem;
		sPtr declElem;
		stack<sPtr> tagElements;
		attribElems _attributes;
		//size_t pos = 0;

		//helper functions
		void makeElement();
		void makeDocElementChild();
		void makeProcChild();
		bool makeTaggedElemChild();
		bool isNotTextElem();
		bool isDeclareElem();
		bool isProcElem();
		bool isCommentElem();
		bool isTagCloseElem();
		bool isTagSelfCloseElem();
		bool hasAttribute();
		std::string extractTag();
		std::string extractIdentifier(const string & str, size_t& pos);
		attribElems attributes(const string & str);
	};

	using sPtr = std::shared_ptr < AbstractXmlElement >;
}
#endif
