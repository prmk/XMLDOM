#ifndef EXECUTIVE_H
#define EXECUTIVE_H
/////////////////////////////////////////////////////////////////////////////
// Executive.h - Interaction between the user and the system			   //
//																		   //
// ver 2.0                                                                 //
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
Module Operations
-----------------
This module acts an interaction between the user and the system.

It interacts with the XMLDocument to process the data.

It informs the Display package to show the results to the user


Public Interface
----------------
Executive exec
exec.XMLDOM
exec.display;


Required Files
--------------
Executive.h, Executive.cpp


Maintenance History
-------------------
ver 2.0:
- Code to show requirements 6, 7, 8, 9

ver 1.0
- Code to show requirements 3, 4, 5, 10
*/
#include "DisplayHandler.h"
#include "XMLDocument.h"
#include "FileSystem.h"
#include "XMLElement.h"

using namespace FileSystem;
using namespace std;

namespace XMLDocumentModel
{
	class Executive
	{
	public:
		
		Executive();
		Executive(const std::string& src, bool isFile = true);
		~Executive();
		DisplayHandler display;
		XmlDocument *XMLDOM;
		string writeToString();
		string writeToFile();
	private:
	};
}

#endif