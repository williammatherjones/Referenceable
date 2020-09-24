/*
 * Reference.cpp
 *
 *  Created on: Sep 15, 2020
 *      Author: billjones
 */

#include "Referenceable.h"

namespace ref {

Referenceable::Referenceable () {
	m_refCount = 0;
}

void Referenceable::addRef () {
	m_refCount++;
}
void Referenceable::release () {
	m_refCount--;
}

int Referenceable::getReferenceCount () const{
	return m_refCount;
}

} /* namespace ref */
