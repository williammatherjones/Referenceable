# Referenceable
A C++ sample project demonstrating an innovative approach to handling the memory management of class instances.

<b>Adjective</b>
Capable of being referenced.<br>
<b>Origin</b>
reference + -able<br>
## Overview
This project addresses a fundamental problem in the C++ language that makes programming a challenge, especially for developers proficient in Java trying to learn C++.  While C++ code might look similar to Java, the details about how it works are quite different.  While Java developers can use new to create class instances knowing that the garbage collector will eventually clean up after them, the C++ developer has no such luxury.  This can make it difficult to train Java developers to write production C++ code without introducing defects that are particularly difficult to debug.  This project demonstrates a better approach that creates a clean and easy to understand object referencing model that is natural for Java developers.   It also has the advantage of removing the tedium for seasoned C++ developers, allowing them to write high quality production code faster.

Before describing the solution, let’s review the problem it addresses.
