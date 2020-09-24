/*
 * Loader.h
 *
 *  Created on: Sep 21, 2020
 *      Author: billjones
 */

#ifndef FILELOADER_H_
#define FILELOADER_H_

#include <string>
#include <list>

#include "../model/StringList.h"

using namespace std;

namespace fileIO {

#define NAME_LENGTH 10

/*
 * Used to open and read a file.  The implementation assumes the file contains a list of fixed length
 * stings.  The length of each field is defined by NAME_LENGTH
 *
 */
class FileLoader {
public:
	FileLoader(string fileName);
	~FileLoader();

	/*
	 * Reads the contents of the file and creates a StringList containing the values.
	 * Returns a reference to the new StringList
	 */
	model::StringListRef createStringList ();

	const string getFileName ()		{ return m_fileName; }

private:
	string 			m_fileName;
};

} /* namespace fileIO */

#endif /* FILELOADER_H_ */
