//============================================================================
// Name        : My Learning Project.cpp
// Author      : Bill Jones
// Version     :
// Copyright   : Your copyright notice
// Description : Example of how reference classes can clean up
// 				 memory management of class instances allocated
//				 using the new operator.
//============================================================================

#include <iostream>
#include <string>

#include "loader/FileLoader.h"
#include "model/StringList.h"
#include "reference/Referenceable.h"
#include "view/StringListView.h"

using namespace std;

/*
 * The name of the file used to load string values into the list.
 */
#define STRING_LIST_FILE_NAME "src/Names.txt"

view::StringListViewRef loadPrintAndMergeLists ();
model::StringListRef 	loadList 			   ();
void 					printAndSortList 	   (view::StringListViewRef listViewRef);
void 					mergeAdditionalValues  (view::StringListViewRef listViewRef);

/*
 * Main entry point for program this program.
 *
 * The listViewRef reference class automatically deletes its class instance
 * once all references are released.  It will delete at the end of the function
 * when the listViewRef destructor is called.
 * See:  Referencable.h for more information about reference classes.
 */
int main() {
	view::StringListViewRef listViewRef = loadPrintAndMergeLists ();

	if (listViewRef == NULL) {
		cout << "main: listViewRef is NULL";
		return -1;
	}

	cout << "\nThe file model and view will be destructed\n";
	// Clean up:  No need to delete pointers.  Once the reference classes go out of scope, they are automatically deleted.
	return 0;
}

/*
 * Performs the steps implied in the function name. Steps are:
 * 1. Load the list from a file
 * 2. Print and sort the list
 * 3. Merge additional values into the list
 *
 * The listRef and listViewRef reference classes automatically delete all class instances
 * created once all references are released.  Since the listViewRef has a reference to the
 * listRef, both instances still exist at the end of the function.   Returning the listViewRef
 * causes the Reference class copy constructor to be called, which in turn adds a reference.
 * See:  Referencable.h for more information about reference classes.
 */
view::StringListViewRef loadPrintAndMergeLists () {
	model::StringListRef  	listRef = loadList ();

	if (listRef == NULL) {
		cout << "loadPrintAndMergeLists: listRef is NULL";
		return NULL;
	}
	view::StringListViewRef listViewRef = new view::StringListView (listRef);

	printAndSortList (listViewRef);
	mergeAdditionalValues (listViewRef);
	return listViewRef;
}

/*
 * Loads the file referenced by STRING_LIST_FILE_NAME and creates a StringList.
 */
model::StringListRef loadList () {
	fileIO::FileLoader 		myLoader (STRING_LIST_FILE_NAME);
	model::StringListRef  	listRef;

	try {
		listRef = myLoader.createStringList();
	} catch (int error) {
		return NULL;
	}
	return listRef; // Success
}


/*
 * Prints the contents of the list to stdout, sorts it and prints again
 */
void printAndSortList (view::StringListViewRef listViewRef) {
	model::StringListRef  	listRef = listViewRef->getStringList();

	listViewRef->printNames ("List loaded from file");
	cout << "Sorting the list\n";
	listRef->sort ();
	listViewRef->printNames ("List loaded from file");
}

/*
 * Creates a StringList with values from memory and merges them into the main StringList
 * loaded from a file.  It prints the list at each step to trace the progress.
 */
void mergeAdditionalValues (view::StringListViewRef listViewRef) {
	model::StringListRef  	listRef 			= listViewRef->getStringList();
	model::StringListRef 	insertListRef 		= new model::StringList ();
	view::StringListViewRef insertListViewRef 	= new view::StringListView (insertListRef);

	string additionalValues [] = {"Strawberry", "Kiwi", "Raspberry"};

	insertListRef->append (3, additionalValues);
	insertListViewRef->printNames ("Insert list");

	cout << "Sorting the insert list\n";
	insertListRef->sort ();
	insertListViewRef->printNames ("Insert list");

	cout << "\nMerging...\n";
	listRef->mergeStringList (insertListRef);

	listViewRef->printNames ("List after merge");
	insertListViewRef->printNames ("Insert list is unchanged (After merge)");

	cout << "\nThe additional model and view will be destructed\n";
}
