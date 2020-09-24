/*
 * Loader.cpp
 *
 *  Created on: Sep 21, 2020
 *      Author: billjones
 */

#include "FileLoader.h"

#include <iostream>
#include <fstream>

namespace fileIO {

FileLoader::FileLoader(string fileName) {
	m_fileName = fileName;
}

FileLoader::~FileLoader() {
}

/*
 * Reads the contents of the file referred to by m_fileName and loads them into a NameList.
 * Returns a Reference class.  If createStringList is called without assigning to a reference
 * class, it will be deleted immediately, which prevents any possibility of memory leaks.
 */
model::StringListRef FileLoader::createStringList () {
	ifstream inputStream;
	model::StringListRef nameListRef = new model::StringList ();

	inputStream.open (m_fileName, std::ifstream::in);
	if (inputStream.fail()) {
		// Something went wrong during open.  Permissions - bad file name?
		cout << "Error:  Unable to read \"" << m_fileName << "\"\n";
		throw -1;
	}

	// get length of file:
	inputStream.seekg (0, inputStream.end);
	int fileLength = inputStream.tellg();
	int nameCount = fileLength / NAME_LENGTH;
	int remainingChars = fileLength % NAME_LENGTH;
	if (remainingChars != 0) {
		cout << "Warning:  File length is not a multiple of " << NAME_LENGTH << ". Possible data corruption.\n";

	}
	// Go back to the file beginning
	inputStream.seekg (0, inputStream.beg);

	char buffer [NAME_LENGTH];

	for (int i=0; i < nameCount; i++) {
		inputStream.read(buffer, NAME_LENGTH);
		nameListRef->append (buffer);
	}
	cout << "Loaded the list from [" << m_fileName << "].\n";
	return nameListRef;
}

} /* namespace fileIO */
