SmartPointer
============

C++ SmartPointer for memory management

    NOTE: There could be a lot of bugs on code. Unfortunately I don't have any time to fix them. I put my code here for inspiration to related developers.

Common mistakes which occur with pointers in C++ are:
* Copy functions could leads to memory leaks. So, we must be careful while using operator=() and copy constructor.
* Polymorphic arrays could lead to errors.
* During inheritance, if base class has no virtual destructors, this could lead memory leaks.
* If new operator is used with delete[] operator or if new[] operator is used with delete operator, this leads to undetermined situations.
* If new operator is used with free function or if malloc function is used with delete operator, this leads to undetermined situations.
* Incorrect casting may lead to errors.
* If a function returns a pointer that points a local variable, there will be crash on the program.
* Uninitialized pointers lead to errors.
* If a program has different kinds of exit ways like exceptions or if-else structures, this could lead memory leaks.
* If memcpy function is used for copying to intersected memory blocks, this could leads errors or memory leaks.

In this work, I aim to patch the defined errors and missing parts of C++. My work is simplifying programmers to focus on their codes, decreases memory leaks, and makes simple code interface for pointers. 

Smart Pointer works like any normal C++ pointer except that it is a virtual class. My Smart Pointer will be a template class so; we can use Smart Pointer for every kind of variable, class or structure.

Benefits of Smart Pointer are:
* Programmer will not handle with any allocation or recycling work. Because of that, it will be easier to focus on the main problem.
* Any errors will be control with exception mechanism. Because of that, errors will not affect the program flow.
* Every structure, code or operation will be written inside the Smart Pointer class. So, this effective structure is providing us to unnecessary allocations and recycling.

We defined some common C++ pointer errors in the beginning. My Smart Pointer will guarantee that, 1, 3, 4, 5, 8, 9’s mistakes will not be occur if programmer use Smart Pointer.
