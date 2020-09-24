/*
 * NameList.h
 *
 *  Created on: Sep 21, 2020
 *      Author: billjones
 */

#ifndef NAMELIST_H_
#define NAMELIST_H_

#include <string>
#include <list>

#include "../reference/Referenceable.h"

using namespace std;

namespace model {

/*
 * Implements a simple list of string values which can be sorted and merged with other lists.
 */
class StringList : public ref::Referenceable {
public:
	StringList();
	virtual ~StringList();

	/*
	 * Appends value to the end of the list.
	 */
	void append (string value);
	/*
	 * Appends each value to the end of the list.
	 * The array size is specified because of the challenges of
	 * determine the number of elements in an array in C++.
	 */
	void append (int arraySize, string values []);

	/*
	 * Returns the interal std::list class that is used for storage.
	 */
	const list<string> &getInternalList ()	{ return *m_pValues; }
	/*
	 * Returns the number of string values stored in the list.
	 */
	int size () { return m_pValues->size (); }

	/*
	 * Sorts the list alphabetically.  The sorting can be changed from case sensitive to
	 * case insensitive sorting by modifying StringList.cpp.  It is determined by either
	 * #define CASE_SENSITIVE_COMPARE
	 * #undef CASE_SENSITIVE_COMPARE
	 */
	void sort ();

	/*
	 * Merges the values stored in pAdditonalNames into this list.   Each list is sorted
	 * prior to the merge. The case sensitive vs insensitive is controlled using the same
	 * logic and sort:
	 * #define CASE_SENSITIVE_COMPARE
	 * #undef CASE_SENSITIVE_COMPARE
	 */
	void mergeStringList (StringList *pAdditionalNames);
	/*
	 * Determines if the list is sorted by iterating through the list checking the previous
	 * and next values.   The case sensitive vs insensitive is controlled using the same
	 * logic and sort:
	 * #define CASE_SENSITIVE_COMPARE
	 * #undef CASE_SENSITIVE_COMPARE
	 */
	bool isSorted ();

private:
	list<string> *	m_pValues;
};

DECLARE_REFERENCE(StringList);

} /* namespace model */

#endif /* NAMELIST_H_ */
