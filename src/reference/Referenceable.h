/*
 * Referenceable.h
 * Class Referenceable is used to allow multiple references to the same pointer, and have the class automatically delete itself after the
 * final reference is released.   This dramatically simplifies the logic required to manage memory for class instances used by multiple
 * classes.
 *
 * Care must be taken to avoid circular references.  In cases of bi-directional associations, one end must "own" the reference with a
 * call to addRef.   The other end must be cleaned up on release.   An example is parent / child or prev / next.  The parent and previous
 * instance can call addRef, whereas the child and next do not.
 *
 * Declaration:
 * Reference counted classes must inherit ref::Referenceable.   The Reference class is declared using the DECLARE_REFERENCE macro.
 * The reference class name will be in the form of ParamValue##Ref.  For example, the code below defines StringListRef.  The convention
 * is to use the referenced class name as the macro parameter.
 *
 * class StringList : public ref::Referenceable {
 * };
 * DECLARE_REFERENCE(StringList);
 *
 * Usage:
 * StringListViewRef myClassFactory () {
 * 	return new StringListView ();
 * }
 *
 * void someFunction () {
 * 	StringListViewRef viewRef = myClassFactory ();
 * 	// No need for cleanup.
 * 	// myClassRef will delete the instance when it destructs!
 * }
 *
 *  Created on: Sep 15, 2020
 *      Author: billjones
 */

#ifndef REFERENCE_REFERENCEABLE_H_
#define REFERENCE_REFERENCEABLE_H_

#include <iostream>

namespace ref {

/*
 * Adds the reference count ability to any C++ class.
 *
 * Note that addRef and release are private methods.  The friend declaration allows the reference classes
 * to call.  No other access is allowed, which prevents developers using the implementation to accidentally
 * introduce bugs.
 *
 */
class Referenceable {
protected:
	Referenceable ();

public:
	int getReferenceCount () const;

private:
	void addRef ();
	void release ();

private:
	unsigned int m_refCount;
	template <class T> friend class Reference;
};

/*
 * Implements the reference class.  Each subclass of Referencable provides a typedef declaration of the class using
 * the DECLARE_REFERENCE macro.   The template class T is the Referenceable subclass.
 */
template <class T> class Reference {
public:
	/* void constructor allowing an uninitialized reference class to be declared.
	*/
	Reference() {
		m_pInstance = NULL;
	}
	/* T* constructor allowing the reference class to have an initial value
	*/
	Reference(T* pInstance) {
		m_pInstance = NULL;
		setInstance (pInstance);
	}
	/* Destructor required to release reference count.
	*/
	~Reference() {
		releaseInstance (m_pInstance);
	}

	/* Copy constructor allows reference classes to be passed as parameters and returned.
	 * If omitted, m_pInstance will not be copied.
	*/
	Reference(const Reference &other) {
		m_pInstance = NULL;
		setInstance (other.getInstance());
	}

	/* Assignment operator allows one reference class to be assigned to another.
	 * If omitted, m_pInstance will not be copied.
	*/
	Reference& operator=(const Reference &otherInstance) {
		setInstance (otherInstance.getInstance ());
		return *this;
	}

	/* Typecast operator allows typecast of reference class into pointers to the instance class.
	*/
	operator T* () {
		return m_pInstance;
	}
	/* Member selection operator allows methods of the instance class to be called using the
	 * reference class.
	 *
	 * Examples:
	 * reference.hasInstance () - call to reference class
	 * reference->print () 		- call to instance class
	*/
	T* operator->() {
		return m_pInstance;
	}

	/* Used to determine if the reference is set
	 * Returns:
	*/
	bool hasInstance () const {
		return m_pInstance != NULL;
	}
	/* Returns the underlying m_pInstance pointer
	*/
	T* getInstance () const {
		return m_pInstance;
	}

private:
	/* Does the common logic required to assign a new instance including releasing any
	 * existing instance
	*/
	void setInstance (T* pInstance) {
		if (pInstance == m_pInstance) {
			// Assigning the current value.   Don't release and addRef because the release may destruct m_pInstance
			return;
		}
		if (m_pInstance != NULL) {
			releaseInstance (m_pInstance);
		}
		m_pInstance = pInstance;
		if (m_pInstance != NULL) {
			m_pInstance->addRef ();
		}
	}

	/* Does the common logic required to release the current instance.  If the reference count reaches zero,
	 * the instance is deleted.
	*/	void releaseInstance (T* pInstance) {
		if (pInstance != NULL) {
			pInstance->release ();

			if (pInstance->getReferenceCount () == 0) {
				std::cout << "Reference::releaseInstance - Reference count reached zero.  Deleting instance " << pInstance << "\n";

				delete pInstance;
			}
		}
	}

private:
	T* m_pInstance;
};
#define DECLARE_REFERENCE(T) typedef ref::Reference <T> T##Ref;

} /* namespace ref */

#endif /* REFERENCE_REFERENCEABLE_H_ */
