/*
 * NameListView.h
 *
 *  Created on: Sep 22, 2020
 *      Author: billjones
 */

#ifndef VIEW_STRINGLISTVIEW_H_
#define VIEW_STRINGLISTVIEW_H_

#include "../model/StringList.h"
#include "../reference/Referenceable.h"

namespace view {

/*
 * Implements the logic required to print output to stdout using a StringList using
 * the model/view pattern.  Separating the view from the string list allows other
 * implementations of views without impacting the model implementation.
 */
class StringListView : public ref::Referenceable {
public:
	StringListView(model::StringList *pStringList);
	virtual ~StringListView();

	void printNames (string label);

	model::StringList *getStringList () { return m_StringListRef; }

private:
	/*
	 * Reference class guarantees the StringList will not be destructed while referenced here.
	 */
	model::StringListRef m_StringListRef;
};

DECLARE_REFERENCE(StringListView);

} /* namespace view */


#endif /* VIEW_STRINGLISTVIEW_H_ */
