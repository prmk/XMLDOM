/////////////////////////////////////////////////////////////////////////////
// DisplayHandler.cpp - Displays the input and the output on the console     //
//																		   //
// ver 1.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Pramukh Shenoy, 2015                                        //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Yoga Pro 2												   //
// Application: OOD Project, 2015	                                       //
// Author:      Pramukh Shenoy,Syracuse University                         //
//              (315) 746-0822, pbantwal@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
NOTE:
Test Stubs for this package needs data from the Executive and hence,
all the functions cannot be tested.
*/

#include "DisplayHandler.h"
#include <iostream>
#include <string>
using namespace XMLDocumentModel;

//copies the tree into the local object
void DisplayHandler::setDOM(sPtr dom)
{
	pDocElement_ = dom;
}

//Displays a title on the console
void DisplayHandler::title(const string& title, char ch)
{
	cout << endl << endl << title;
	cout << endl << string(title.size() + 2, ch);
	cout << endl;
}

// indicates the start of a requirement.
// used for requirement demo
void XMLDocumentModel::startReq(char underlineChar)
{
	std::cout << "\n" << std::string(200, underlineChar);
}

// indicates the end of a requirement.
// used for requirement demo
void XMLDocumentModel::endReq(char underlineChar)
{
	std::cout << std::string(200, underlineChar) << endl << endl << endl;
}

//prints a text on the console
void XMLDocumentModel::text(const string& title)
{
	cout << title;
}

//prints the content of the tree in a tree structure on the console
void DisplayHandler::printTree()
{
	if (pDocElement_ != nullptr)
	cout << pDocElement_->toString();
	cout << endl;
	cout << endl;
}

//prints the content of the tree in a tree structure on the console
void DisplayHandler::printTree(sPtr dom)
{
	pDocElement_ = dom;
	printTree();
}

//prints the content of an element in a tree on the console
void DisplayHandler::printElement(std::vector<sPtr> found)
{
	if (found.empty())
		cout << "Null pointer returned. The search query did not match data" << endl;
	else
	{
		for (auto elem : found)
			cout << elem->toString() << endl;
	}
}

//prints the content of an element in a tree on the console
void DisplayHandler::printElement(attribElems attributes)
{
	cout << endl;
	if (attributes.empty())
		cout << "Null pointer returned. No attributes found in the selected element" << endl;
	else
	{
		for (auto elem : attributes)
			cout << "Name: '" << elem.first << "'" << " Value: '" << elem.second << "'" << endl;
	}
}

//prints the output based on the status
//True indicates success and the same is printed on the screen
//False indicates failure and the same is printed on the screen
void DisplayHandler::printStatus(bool status)
{
	if (status)
		cout << "Success. The function returned true." << endl;
	else
		cout << "Failure. The function returned false." << endl;
}


//----< test stub >----------------------------------------------------------
#ifdef TEST_DISPLAY

int main(int argc, char *argv[])
{
	DisplayHandler display;
	display.title("Test Stub", '=');
	startReq();
	cout << endl;
	text("Demo text");
	cout << endl << endl;
	display.printStatus(true);
	cout << endl;
	display.printStatus(false);
	endReq();
}

#endif