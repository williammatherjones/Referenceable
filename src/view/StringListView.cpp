/*
 * NameListView.cpp
 *
 *  Created on: Sep 22, 2020
 *      Author: billjones
 */

#include "StringListView.h"

namespace view {

StringListView::StringListView(model::StringList *pStringList) {
	m_StringListRef = pStringList;
}

StringListView::~StringListView() {
	// No need to clean up m_NameListRef.  The reference class guarantees it will be cleaned up.
}

void StringListView::printNames (string label) {
	printf ("\n%s:\n", label.c_str());
	const list<string> &internalNames = m_StringListRef->getInternalList();
	for (list<string>::const_iterator it = internalNames.cbegin (); it != internalNames.cend(); ++it) {
		printf ("\t[%s]\n", it->c_str());
	}
	printf ("%i entries (",m_StringListRef->size());
	if (m_StringListRef->isSorted ()) {
		printf ("Sorted");
	} else {
		printf ("Not sorted");
	}
	printf (")\n");
}

} /* namespace view */
