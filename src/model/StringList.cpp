/*
 * NameList.cpp
 *
 *  Created on: Sep 21, 2020
 *      Author: billjones
 */

#include "StringList.h"

#include <iostream>


namespace model {

StringList::StringList() {
	m_pValues = new list<string> ();
}

StringList::~StringList() {
	delete m_pValues;
}

void StringList::append (int arraySize, string names []) {
	if (names != NULL) {
		for (int i=0; i< arraySize; i++) {
			append (names[i]);
		}
	}
}

void StringList::append (string name) {
	m_pValues->push_back(name);
}

#define CASE_SENSITIVE_COMPARE
//#undef CASE_SENSITIVE_COMPARE

#ifdef CASE_SENSITIVE_COMPARE
// comparison, case sensitive.
bool isLessThan (const std::string& rightValue, const std::string& leftValue)
{
  return rightValue < leftValue;
}

#else
// comparison, not case sensitive.
bool isLessThan (const std::string& rightValue, const std::string& leftValue)
{
  unsigned int i=0;
  while ( (i<rightValue.length()) && (i<leftValue.length()) )
  {
    if (tolower(rightValue[i])<tolower(leftValue[i])) {
    	return true;
    } else if (tolower(rightValue[i])>tolower(leftValue[i])) {
    	return false;
    }
    i++;
  }
  return ( rightValue.length() < leftValue.length() );
}
#endif

void StringList::sort () {
	m_pValues->sort (isLessThan);
}

bool StringList::isSorted () {

	if (m_pValues->size () < 2) {
		// Lists with zero or one name are sorted by definition
		return true;
	}
	/* This loop is a bit more complex than the standard list iteration.  It keeps track
	   of two consecutive iterators, allowing it to verify that each previous is less than
	   the next.   Note that it does this with the isLessThan function, allowing the implementation
	   to be changed from case sensitive to case insensitive.   This keeps the comparison used for
	   sorting and merging consistent with the isSorted implementation.
	*/
	list<string>::const_iterator prevIterator = m_pValues->cbegin ();
	list<string>::const_iterator nextIterator = prevIterator;
	for (nextIterator = ++nextIterator;
		 nextIterator != m_pValues->cend();
		 ++nextIterator, ++prevIterator) {

		if (!isLessThan (*prevIterator, *nextIterator)) {
			return false;
		}
	}
	return true;
}

void StringList::mergeStringList (StringList *pAdditionalNames) {
	// Both lists must be sorted in order for the merge to work correctly
	pAdditionalNames->sort ();
	this->sort();

	list<string> additionalInternalNames = pAdditionalNames->getInternalList ();
	m_pValues->merge (additionalInternalNames, isLessThan);
}

} /* namespace model */
