#ifndef DISPLAYHANDLER_H
#define DISPLAYHANDLER_H
/////////////////////////////////////////////////////////////////////////////
// DisplayHandler.h - Displays the input and the output on the console     //
//																		   //
// ver 1.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Pramukh Shenoy, 2015                                        //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Yoga Pro 2  											   //
// Application: OOD Project, 2015	                                       //
// Author:      Pramukh Shenoy,Syracuse University                         //
//              (315) 746-0822, pbantwal@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
Module Operations
-----------------
This module prints the contents on to the console.

It prints the title for each operation

It prints the contents of the Tree.

It prints the element contents of the current element pointer.

It prints the 

The module is used by the Executive.


Public Interface
----------------
DisplayHandler display;
display.title("Title");
display.printTree();
display.printTree(sPtr dom);
display.printElement(found);
display.printElement(attributes);
display.printStatus(status);

Required Files
--------------
DisplayHandler.h, DisplayHandler.cpp


Build Command
-------------
cl /DTEST_DISPLAY DisplayHandler.cpp


Maintenance History
-------------------
ver 1.2:
- Code to print the output based on the status

ver 1.0:
- Code to print the contents of the tree
- Code to print the element contents
- Code to print the title
*/
#include <memory>
#include <iostream>
#include <fstream>
#include "XMLElement.h"
#include "FileSystem.h"

using namespace std;

namespace XMLDocumentModel
{
	class DisplayHandler
	{
	public:
		using sPtr = std::shared_ptr < AbstractXmlElement >;
		typedef std::vector<std::pair<std::string, std::string>> attribElems;

		void title(const std::string& title, char underlineChar = '-');
		void printTree();
		void printTree(sPtr dom);
		void printElement(std::vector<sPtr> found);
		void printElement(attribElems attributes);
		void printStatus(bool status);
		void setDOM(sPtr dom);
	private:
		sPtr pDocElement_;
	};

	void text(const string& title);
	void startReq(char underlineChar = '-');
	void endReq(char underlineChar = '-');
}

#endif