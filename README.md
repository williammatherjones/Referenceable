# Referenceable
A C++ sample project demonstrating an innovative approach to handling the memory management of class instances.

<b>Adjective</b>
Capable of being referenced.<br>
<b>Origin</b>
reference + -able<br>
## Overview
This project addresses a fundamental problem in the C++ language that makes programming a challenge, especially for developers proficient in Java trying to learn C++.  While C++ code might look similar to Java, the details about how it works are quite different.  While Java developers can use new to create class instances knowing that the garbage collector will eventually clean up after them, the C++ developer has no such luxury.  This can make it difficult to train Java developers to write production C++ code without introducing defects that are particularly difficult to debug.  This project demonstrates a better approach that creates a clean and easy to understand object referencing model that is natural for Java developers.   It also has the advantage of removing the tedium for seasoned C++ developers, allowing them to write high quality production code faster.

Before describing the solution, let’s review the problem it addresses.

## The C++ dilemma: How to manage memory
Memory leaks are among the most insidious of the types of defects that can be introduced into software.   When coding in C++, they are difficult to detect and easy to introduce.  

C++ developers must weigh different options in deciding how to implement their code.   It comes down to the choice of allocating class instances on the stack as local variables and passing by reference, or using the new and delete operators to allocate class instances on the heap.  This is a foreign concept to Java developers because Java only supports one model:  Allocate with new and garbage collect when the object is no longer referenced.

### Using heap memory
On the surface, declaring a pointer to a class and allocating it with the new operator is just like Java usage. But that’s where the similarities end. C++ has no mechanism to call delete at some point in the future. If never called, the program will leak memory. Call it too soon, when other classes still have pointers to the instances, and that may access invalid pointers.  Worse still, it could be valid memory recycled by another allocation, causing writes to the memory to corrupt the other classes.   This class of defect is easy to introduce, difficult to detect and time consuming to fix.  This is the polar opposite of desirable production coding environments in any language. Defects should to be hard to introduce, easy to detect and quick to fix.  

This example demonstrates the dangers of unchecked use of new and delete.

```cpp
class StringListView {
};
StringListView * myClassFactory () {
	return new StringListView ();
}

void someFunction () {
	StringListView* pMyClass = myClassFactory ();
	// Oops!  Forgot to delete pMyClass.  Memory leak.
}
```
The problem is that the author of someFunction needs to know the implementation of myClassFactory.  This causes the code to be more tedious to write.  Furthermore, should the implementation of myClassFactory change in the future, the code could break.   This also causes the lifecycle of the class to have a procedural implementation. In order to understand when to call delete, the author of the code must understand the implementation details, not only of the code that allocates the class, but all code that uses it.  As a result, Object oriented principles of encapsulation, polymorphism and providing interfaces can not be used effectively.

The problems get even worse when choosing exactly when to delete instances.  Here’s another example.
```cpp
void otherFunction (StringListView* pMyClass);

void someFunction () {
	StringListView* pMyClass = myClassFactory ();

	StringListView* pMyOtherClass = myClassFactory ();
	otherFunction (pMyOtherClass);
	delete pMyOtherClass;
	// Oops!  otherFunction stores a copy of the pointer in a data structure.
	// It just became invalid!
}
```
In this case, the developer has responsibly cleaned up by calling delete, but was unaware that the code in otherFunction stored the pointer in a data structure.  Even worse, what otherFunction did not do that when this code was written, but gets refactored at some point in the future to do it?  In order to avoid creating this type of defect, developers need to understand the low level implementation of the entire code base.  

As the code becomes larger and more complex, this eventually becomes unmanageable.  In order to avoid difficult and costly implementation issues, a different approach is needed.

### Using stack memory
This is the obvious alternative at at first would seem like a great solution.  Declaring class instances as local variables is very easy in C++.   However, the author of the code quickly becomes bogged down in implementation details and it can be very easy to introduce another class of memory management defects.   The problem is how to pass those class instances to other methods.  This comes down to the choice of passing by reference or passing a pointer.

This is demonstrated by the following code:
```cpp
void byValue (StringListView myClass) {
}
void byReference (StringListView* pMyClass) {
}
void someFunction () {
	StringListView myClass;

	byValue (myClass);
	byReference(&myClass);
}
```
Calling byValue seems like a simple solution until you understand what C++ actually does to handle it.   Passing values this way requires a copy constructor to be implemented in MyClass.   Any member variables must copy values.   For complicated class implementations that compose member variables using other class instances, requires those classes to also have copy constructors.   Add a member variable and forget to handle it correctly and you will introduce another hard to find defect.   On top of that, this seemingly innocent call to byValue could be very expensive if MyClass were a complex implementation.  

These issues become even more complex when writing code that Java developers take for granted.  Consider this code:
```cpp
StringListView myClassFactory () {
	StringListView myClass;
	return myClass;
}
void dummy () {
	StringListView myClass = myClassFactory ();

	StringListView myOtherClass = myClass;
}
```
Ask a Java developer and they will tell you this is not complex.  But in reality, this code makes four copies of the same class.   Each copy is different.   A Java developer would expect all a method that changes myClass to also change myOtherClass.   

Calling byReference converts the object to a pointer to the object on the stack.  This seems reasonable, but what happens if byReference uses the new operator to allocate a class on the heap that uses the pointer?   The program will have a data structure with a pointer that will become invalid once the calling function or method finishes and the heap memory is freed.   

Now that we have reviewed the tedious mess that C++ developers face, let’s see how the code can be improved using a design pattern called reference classes.
## A different approach: C++ reference classes
Memory leaks are among the most insidious of the types of defects that can be introduced into software.   This project uses a technique that brings the advantages of Java style object references while still maintaining the power and control that C++ developers require.  

Reference classes encapsulate the lifecycle of C++ class instances.  Instances are still allocated with the new operator, but the reference classes decide when to call delete. This transforms the code, removing the sharp edges and rusty nails that typically cause C++ code to leak memory, or excessive use of copy constructors that cause the code to look clean but makes passing and returning classes to be complex and computationally expensive.  

The complexity of reference counting is encapsulated in a template class, which is the basis of the view::StringListViewRef shown below.   
```cpp
class StringListView : public ref::Referenceable {
public:
	void printList ();
};
DECLARE_REFERENCE(StringListView);

StringListViewRef myClassFactory () {
	return new StringListView ();
}

void someFunction () {
	StringListViewRef viewRef = myClassFactory ();
	// No need for cleanup.
	// myClassRef will delete the instance when it destructs!
} 
```
The reference class allows code to be written much more in the style familiar to Java programmers.   Developers no longer need to understand the implementation details of C++ memory management.  The class is simply deleted when it is no longer referenced.  There is a lot going on behind the implementation, but the result is very clean, simple and easy to maintain C++ code.

Let’s make the example slightly more complex.
```cpp
class StringList : public ref::Referenceable {
};
DECLARE_REFERENCE(StringList);

class StringListView : public ref::Referenceable {
public:
	StringListView(StringList *pStringList) {
		m_StringListRef = pStringList;
	}

	void printList () {};
private:
	StringListRef m_StringListRef;
};
DECLARE_REFERENCE(StringListView);

StringListViewRef myClassFactory (StringList *pStringList) {
	return new StringListView (pStringList);
}

StringListRef someFunction () {
	StringListRef listRef = new StringList ();
	StringListViewRef viewRef = myClassFactory (listRef);
	if (viewRef != NULL) {
		viewRef->printList ();
	}
	return listRef;
}
void someOtherFunction () {
	StringListRef listRef = someFunction ();
}
```
The code has several notable changes:   
* The StringList class has been added.  
* The StringListView now has a private member variable.   Since this is a reference class, it ensures that the StringList instance will not be deleted while it is still referencing it.
* The someFunction method now returns the listRef.  When the function returns, the viewRef will delete its StringListView.   When that class destructs, it will release its m_StringListRef reference.   However, the StringList instance will not be deleted because it is still referenced by listRef.  That instance will not be deleted until someOtherFunction returns, causing the last reference to be released.  
* The someFunction method checks viewRef for NULL.  Overloaded operators cause this to check to see if viewRef if referencing a class.   Internally, this checks a pointer for NULL.
* If you look back at the implementation of someFunction, you may notice something strange. 
```cpp
viewRef->printList ();
```
* If this were a normal C++ class, the syntax should be:
```cpp
viewRef.printList ();
```
* The reference class does this by overriding the member selection operator ->.  The printList method is not a method of the reference class.  It is directly calling the method of the StringListView as defined in the sample.  

The reference class overloads operators and the right constructors in order to achieve this simple usage syntax.  The details of the implementation can be seen in src/reference/Referencable.h.
## C++ reference classes vs Java
There are some subtle differences between this C++ reference class implementation and Java.   Most importantly, there is no garbage collection algorithm.  When classes are no longer referenced, they are deleted immediately.   This is in many ways superior to the Java implementation because you can count on destructors being called immediately.   Therefore, classes can reference system resources.  For example, a class can keep a file open, reading and writing as needed.   

However, this does not mean that a garbage collection style reference class could not be implemented. The implementation could be changed to add the pointers to a garbage collection list when the reference count goes to zero. Another thread could monitor and process the list, causing the threads doing the allocation to return faster.  

Furthermore, there is no reason that two different implementations could not be defined, allowing the author of the class a choice between the strict and garbage collection styles of memory management. For example, the author of the StringList class may opt for strict references because the class keeps a file open.
```cpp
class StringList : public ref::Referenceable {
};
DECLARE_REFERENCE(StringList);
```
However, when writing the StringListView, the author decides to delay the destruction using a garbage collection implementation.  
```cpp
class StringListView : public ref::GC_Referenceable {
};
DECLARE_GC_REFERENCE(StringListView);
```
One of the areas yet to be researched is providing implementations of the new and delete operators on the Referenceable class.  This implementation uses the default new and delete for C++ classes.  The C++ language supports the implementation of alternate implementations.  It is certainly possible to overide the new and delete operators for the Referenceable class that customize this behavior.   In fact, this may be a way to acheive different flavors of the Java style garbage collection functionality.  More research is required to determine the exact implementation.   
```cpp

void* operator new(std::size_t sz) {
    // Customize functionality here...
    return std::malloc(sz);
}
void operator delete(void* ptr) noexcept
{
    // Customize functionality here...
    std::free(ptr);
}
```
There are several flavors of garbage collection that could apply to C++ programs:
* Gaurantee the destructor of the class will be called at some point in the future by a different thread.  This would allow the main execution to return faster than the version that includes the execution of all destructors.  This could be accomplished by providing an alternate implementation of the Reference class that defers calling delete by adding the pointer to a list that is shared with a garbage collecion thread.
* Call delete immediately (in the same thread that called new), but delay the call to free by placing the pointer in a list, allowing the garbage collector to call free in a different thread.
* Allow some instances to intentionally “leak” by shutting down without fully destructing all objects and freeing their memory.  Some class implementations may not need formal destruction.  While it is reasonable to assume those classes would not define destructors, there could still be significant execution time saved by not freeing all pointers prior to ending excution of a C++ program.  

This could cause some C++ programs to run faster by skipping deleting class instances while preventing those intentional leaks from exhausting memory.

## Code Review
Start by reviewing the code in My Reference Project.cpp.   The code loads a StringList from a src/Names.txt, sorts the values and then merges values from an additional list loaded from memory.   What the program does is not remarkable, it's how it does it.   

## Console Output
Here is an example of the output written to cout during execution.  
```
Loaded the list from [src/Names.txt].

List loaded from file:
	[Grape     ]
	[Orange    ]
	[Apple     ]
	[Bananna   ]
4 entries (Not sorted)
Sorting the list

List loaded from file:
	[Apple     ]
	[Bananna   ]
	[Grape     ]
	[Orange    ]
4 entries (Sorted)

Insert list:
	[Strawberry]
	[Kiwi]
	[Raspberry]
3 entries (Not sorted)
Sorting the insert list

Insert list:
	[Kiwi]
	[Raspberry]
	[Strawberry]
3 entries (Sorted)

Merging...

List after merge:
	[Apple     ]
	[Bananna   ]
	[Grape     ]
	[Kiwi]
	[Orange    ]
	[Raspberry]
	[Strawberry]
7 entries (Sorted)

Insert list is unchanged (After merge):
	[Kiwi]
	[Raspberry]
	[Strawberry]
3 entries (Sorted)

The additional model and view will be destructed
Reference::releaseInstance - Reference count reached zero.  Deleting instance 0x7fd5b0d041a0
Reference::releaseInstance - Reference count reached zero.  Deleting instance 0x7fd5b0d04160

The file model and view will be destructed
Reference::releaseInstance - Reference count reached zero.  Deleting instance 0x7fd5b0d04140
Reference::releaseInstance - Reference count reached zero.  Deleting instance 0x7fd5b0c057a0
```
## Conclusion
The implementation of Referenceable is a transformative change to development of complex production C++ code.  It removes the staggering complexity required to manage the lifecycle of C++ classes by offering the best of both models of memory management avilable to C++ developers, using objects allocated on the call stack by declaring as a local variable or allocated on the heap using the new operator.  Because of the flexiblity and power of the C++ language, further encapsulation and customization of the implementation is possible through the the new and delete operators, Java style garbage collection, and custom std::allocator implementations. 
