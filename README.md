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
On the surface, declaring a pointer to a class and allocating it with the new operator is just like Java usage. But that’s where the similarities end. C++ has no mechanism to call delete at some point in the future. If never called, the program will leak memory. Call it too soon, when other classes still have pointers to the instances, and that may access invalid pointers.  Worse still, it could be valid memory recycled by another allocation, causing writes to the memory to corrupt the other classes.   This class of defect is easy to introduce, difficult to detect and time consuming to fix.  This is the polar opposite of desirable production coding environments in any language, which need to be hard to introduce, easy to detect and quick to fix.  

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
The problem is that the author of someFunction needs to know the implementation of myClassFactory.  This causes the code to be more tedious to write.  Furthermore, should the implementation of myClassFactory change in the future, the code could break.   This also causes the lifecycle of the class to have a procedural implementation.  

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