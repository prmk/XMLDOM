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
#include "Executive.h"
#include <iostream>
#include <string>

using namespace XMLDocumentModel;
using namespace std;

std::string Prologue = "<?xml version=\"1.0\"?><!-- top level comment -->";
string testXmlString = Prologue + "<Dir><path>/TestFile/Dir1</path><File date = \'notCurrent\'><name type = 'file'>text1.txt< / name> < date>02 / 11 / 2012 < / date > < / File> <File date = \'Current\'><name type = 'xml'>text2.js< / name> < date>02 / 11 / 2015 < / date > < / File> < / Dir>";

//constructor for creating an Executive object
//No class memebers to initialize
Executive::Executive()
{
}

//creates a new XmlDocument based on the users input
//sends the DOM to the Display object which is used to display to user
Executive::Executive(const std::string& src, bool isFile)
{
	XMLDOM = new XmlDocument(src, isFile);
	display.setDOM(XMLDOM->getDOM());
}

//destructor
//deletes the instance of XMLDocument
Executive::~Executive()
{
	delete XMLDOM;
}

//creates a string with data from the XMLDocument 
//and sends it back to the calling object
string Executive::writeToString()
{
	return "Tree Contents\n" + XMLDOM->getDOM()->toString() + "\n";
}

//creates a file in the current directory
//fills it with the contents of the XML DOM
string Executive::writeToFile()
{
	ofstream myfile;
	string fullpath;
	string fileName = "Results.xml";
	FileSystem::Directory::setCurrentDirectory(".");
	string currDir = FileSystem::Directory::getCurrentDirectory();
	if (XMLDOM->getDOM() != nullptr)
	{
		myfile.open(fileName);
		myfile << XMLDOM->getDOM()->toString();
		myfile.close();
		fullpath = currDir + "\\" + fileName;
	}
	return fullpath;
}

//Demonstrates the requirement to build from a file and write to a file
void demonstrateReqFiles(int argc, char* argv[])
{
	title("Demonstrating Req#3 and 4: Reading from a file and Writing to a file");
	for (int i = 1; i < argc; ++i)
	{
		try
		{
			Executive exec(argv[i], true);
			string fileName = argv[i];
			text("Processing file " + fileName);
			exec.display.printTree();
			string path = exec.writeToFile();
			if (path.length() > 0)
				cout << "File created: " << path << endl;
			else
				cout << "Error creating file";
		}
		catch (std::exception ex)
		{
			std::cout << "\n  " << ex.what() << "\n\n";
		}
	}
	endReq();
}

//demonstrates the requirement to read from a string and write to a string
void demonstrateReqStrings()
{
	startReq();
	title("Demonstrating Req#3 and 4: Reading from a string and writing to a string");
	Executive exec(testXmlString, false);
	text("source = "); text(testXmlString); cout << endl << endl;
	text("Contents of the tree");
	exec.display.printTree();
	endReq();
}

//function returns a copy of newly created XML DOM
//return by value
XmlDocument testMoveConstructor(Executive exec)
{
	XmlDocument dom(testXmlString);
	return dom;
}

//demonstrates the requirement of usage of move constructor
//to create a new object
void demonstrateReqMoveConstructor()
{
	startReq();
	title("Demonstrating Req#4: Move ");
	Executive exec;
	std::string Prologue = "<?xml version=\"1.0\"?><!-- top level comment -->";
	std::string test = Prologue;
	test += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";

	XmlDocument doc(test, false);
	text("Contents of Destination XML: ");
	exec.display.setDOM(doc.getDOM());
	exec.display.printTree();

	doc = testMoveConstructor(exec);
	text("Contents of the destination tree after Move");
	exec.display.setDOM(doc.getDOM());
	exec.display.printTree();

	endReq();
}

//function returns a copy of newly created XML DOM
//return by value
XmlDocument testMoveAssignmentOperator(Executive exec)
{
	XmlDocument doc(testXmlString, false);
	return doc;
}

//demonstrates the requirement of usage of move assignment operator
//to create a new object
void demonstrateReqMoveAssOprtr()
{
	startReq();
	title("Demonstrating Req#4: Move Assignment Operator");
	Executive exec;

	std::string Prologue = "<?xml version=\"1.0\"?><!-- top level comment -->";
	std::string test = Prologue;
	test += "<parent att1='val1' att2='val2'><child1 /><child2>child2 body<child1 /></child2></parent>";

	XmlDocument doc(test, false);
	text("Contents of Destination XML: ");
	exec.display.setDOM(doc.getDOM());
	exec.display.printTree();

	doc = testMoveAssignmentOperator(exec);
	text("Calling the Move Assignment Operator. The new Contents of Destination XML:");
	exec.display.setDOM(doc.getDOM());
	exec.display.printTree();

	endReq();
}

//demonstrates the usage of Depth first search for an element
//it prints out the results on the console
void demonstrateReqDFS()
{
	startReq();
	title("Demonstrating Req: DFS");
	Executive exec;
	XmlDocument doc(testXmlString, false);
	text("Contents of XML Document: ");
	exec.display.setDOM(doc.getDOM());
	exec.display.printTree();
	doc.DFS_r_ltr(doc.getDOM());
	endReq();
}

//demonstrates the requirement to search by tag 
//calls the element() function in the XDocument.cpp file
void demonstrateReqElement()
{
	startReq();
	title("Demonstrating Req#6: Search for Element by Tag Name and return the first match");
	Executive exec;
	XmlDocument doc(testXmlString, false);
	text("Searching for tag 'author' by the use of chaining doc.element(\"author\").select()");
	exec.display.printElement((doc.element(doc.getDOM(), "author")).select());
	doc.setFoundNull();
	cout << endl;
	text("Searching for tag 'authorNA' by the use of chaining doc.element(\"authorNA\").select()");
	cout << endl;
	exec.display.printElement((doc.element(doc.getDOM(), "authorNA")).select());
	endReq();
}

//demonstrates the requirement to search by tag 
//calls the elements() function in the XDocument.cpp file
void demonstrateReqElements()
{
	startReq();
	title("Demonstrating Req#6: Search for Element by Tag Name and return all the matches");
	Executive exec;

	XmlDocument doc(testXmlString, false);
	text("Searching for tag 'title' by the use of chaining doc.elements(\"title\").select()");
	exec.display.printElement((doc.elements(doc.getDOM(), "title")).select());
	doc.setFoundNull();
	cout << endl;
	text("Searching for tag 'titleNA' by the use of chaining doc.elements(\"titleNA\").select()");
	cout << endl;
	exec.display.printElement((doc.elements(doc.getDOM(), "titleNA")).select());

	endReq();
}

//demonstrates the requirement to search by attribute name 
//calls the attributeByName() function in the XDocument.cpp file
void demonstrateReqGetElementByID()
{
	startReq();
	title("Demonstrating Req#5: Get Element By ID");
	Executive exec;

	XmlDocument doc(testXmlString, false);
	text("Searching for id 'Company' by the use of chaining doc.attributes(\"Company\").select()");
	exec.display.printElement((doc.attributeByName(doc.getDOM(), "Company")).select());
	doc.setFoundNull();
	cout << endl;
	text("Searching for tag 'CompanyNA' by the use of chaining doc.element(\"CompanyNA\").select()");
	cout << endl;
	exec.display.printElement((doc.attributeByName(doc.getDOM(), "nameNA")).select());

	endReq();
}

//demonstrates the requirement to fetch all the children of a tag
//calls the element() function in the XDocument.cpp file
void demonstrateReqChildren()
{
	startReq();

	title("Demonstrating Req#8: Get Children");
	Executive exec;
	XmlDocument doc(testXmlString, false);
	text("Searching for children of element with tag 'reference' by doc.children(\"reference\")).select()");
	exec.display.printElement((doc.children("reference")).select());
	cout << endl;
	text("Searching for children with element from found_[0]");
	exec.display.printElement((doc.children("")).select());
	cout << endl;

	doc.setFoundNull();
	text("Searching for children of tag 'reference' by chaining - doc.element(\"reference\").children().select()");
	exec.display.printElement(doc.element(doc.getDOM(), "reference").children().select());
	cout << endl;

	endReq();
}

//demonstrates the requirement to add an attribute to an element
//demonstrates the requirement to remove an attribute from an element
void demonstrateReqAddAttribute()
{
	startReq();

	title("Demonstrating Req#9: Add and Remove Attribute");
	Executive exec;
	XmlDocument doc(testXmlString, false);
	text("Adding attribute 'loc=Syracuse' and 'CODE=13210' to element with tag 'publisher'");
	bool status = doc.elements(doc.getDOM(), "publisher").addAttribute("loc", "Syracuse");
	status = doc.elements(doc.getDOM(), "publisher").addAttribute("CODE", "13210");
	cout << endl;
	exec.display.printStatus(status);
	if (status)
	{
		text("The updated contents of the tree ");
		exec.display.printTree(doc.getDOM());
	}
	text("Removing attribute 'loc=Syracuse' from element with tag 'publisher'");
	status = doc.elements(doc.getDOM(), "publisher").removeAttribute("loc");
	cout << endl;
	exec.display.printStatus(status);
	if (status)
	{
		text("The updated contents of the tree ");
		exec.display.printTree(doc.getDOM());
	}
	
	endReq();
}

//demonstrates the requirement to display the list of attributes for any element
void demonstrateReqAttributesList()
{
	startReq();

	title("Demonstrating Req#8: Attributes List");
	Executive exec;
	XmlDocument doc(testXmlString, false);
	text("Listing attributes of element with tag 'page'");
	exec.display.printElement(doc.elements(doc.getDOM(), "page").attributesList().selectAttributes());
	cout << endl;
	doc.setFoundNull();
	text("Listing attributes of element with tag 'pageNA'");
	exec.display.printElement(doc.elements(doc.getDOM(), "pageNA").attributesList().selectAttributes());

	endReq();
}

//demonstrates the requirement to add an child to an element
//demonstrates the requirement to remove a child from an element
void demonstrateReqAddChild()
{
	startReq();
	title("Demonstrating Req#7: Add Child element");
	Executive exec;
	XmlDocument doc(testXmlString, false);
	text("Adding the tagged element <month> to the element with tag 'reference'");
	cout << endl;
	XMLDocumentModel::sPtr child = makeTaggedElement("month");
	child->addChild(makeTextElement("May"));
	bool status = doc.element(doc.getDOM(), "reference").addChild(child);
	exec.display.printStatus(status);
	if (status)
	{
		text("The tree contents are now: ");
		exec.display.printTree(doc.getDOM());
	}
	doc.setFoundNull();
	text("Adding the tagged element <month> to the element with id 'index'");
	cout << endl;
	status = doc.attributeByName(doc.getDOM(), "index").addChild(child);
	exec.display.printStatus(status);
	if (status)
	{
		text("The tree contents are now: ");
		exec.display.printTree(doc.getDOM());
	}
	doc.setFoundNull();
	text("Removing the element 'Author' of the element with tag 'reference1'");
	cout << endl;
	status = doc.element(doc.getDOM(), "reference").removeChild("author");
	exec.display.printStatus(status);
	if (status)
	{
		text("The tree contents are now: ");
		exec.display.printTree(doc.getDOM());
	}
	doc.setFoundNull();
	text("Removing the element 'month' of the element with id 'index'");
	cout << endl;
	status = doc.attributeByName(doc.getDOM(), "index").removeChild("month");
	exec.display.printStatus(status);
	if (status)
	{
		text("The tree contents are now: ");
		exec.display.printTree(doc.getDOM());
	}
	endReq();
}

//demonstrates the requirement to add a root to an empty DOM
void demonstrateReqEmptyRoot()
{
	startReq();
	title("Demonstrating Req#7: Add Root to empty document tree");
	Executive exec;
	string str = "<test>empty DOC</test>";
	XmlDocument doc(str, false);
	text("Adding " + str + " to the empty Document. The tree contents are now: ");
	exec.display.printTree(doc.getDOM());
}

//function to allow the user to press ENTER 
//before the execution can continue
bool userInput()
{
	string userInput;
	cout << "\nPress ENTER to check further requirements";
	getline(cin, userInput);
	cout << endl << endl << endl;
	if (userInput.length() >= 0)
		return true;
	return false;
}

//If the user has provided an input file, 
//the data from the file is used an a demonstration for all the requirements
void readTextFromInputFile(char* argv)
{
	FileSystem::Directory::setCurrentDirectory(".");
	string currDir = FileSystem::Directory::getCurrentDirectory();
	File file(currDir + "\\" + argv);
	file.open(File::in);
	if (file.isGood())
		testXmlString = file.readAll();
	file.close();
}

//The Main executive function which integrates all the requirements of the project
//it handles displaying the data on the console
int main(int argc, char* argv[])
{
	title("                          XML DOCUMENT MODEL                          ", '=');

	try
	{
		if (argc > 1)
			readTextFromInputFile(argv[1]);

		demonstrateReqFiles(argc, argv);
		demonstrateReqStrings();
		demonstrateReqMoveConstructor();
		demonstrateReqMoveAssOprtr();
		demonstrateReqGetElementByID();
		demonstrateReqElement();
		demonstrateReqElements();
		if (userInput())
			demonstrateReqEmptyRoot();
		demonstrateReqAttributesList();
		demonstrateReqChildren();
		demonstrateReqAddAttribute();
		if (userInput())
			demonstrateReqAddChild();
	}
	catch (exception)
	{
		cout << "Error has occurred";
	}
}
