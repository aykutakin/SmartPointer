/****************************************************************************
* Developer: Aykut Akin  													*
*																			*
* The code is implemented under Windows 7 OS with Visual C++ 2010 Express	*
* The code also tested under Ubuntu 10.10 OS								*
*																			*
* Description: Smart pointer library for C++								*
*																			*
****************************************************************************/

#ifndef __SMART_MEMORY_H__
#define __SMART_MEMORY_H__

#include <cstring>

template<class T> class SmartMemory {

private:
	T* ptr; // Pointer's address
	int size; // Size

	T* allocMem(int nSize) { // Allocates memory according to size
		if(nSize == 0)
			return (0);
		else if(nSize == 1)
			return (new T);
		else
			return (new T[nSize]);
	}

	void changeMemSize(int nSize, int first=0) {
		T* nPtr = allocMem(nSize); // Allocate new memory space
		if(nSize > 0)
			std::memcpy(nPtr, ptr+first, size); // Copy old memory values
		freeMem(true); // Release old memory
		ptr = nPtr; // Set ptr
		size = nSize; // Set size
	}

public:

	void init(int nSize) {
		ptr = allocMem(nSize); // Allocate memory
		size = nSize; // Set size
	}

	void freeMem(bool isNew) { // Delete memory according to size
		if(isNew) {
			if(size == 0);
			else if(size == 1)
				delete ptr;
			else
				delete [] ptr;
		}
	}

	int getSize() const { // Return size
		return size;
	}

	T* getPtr() const { // Return pointed address
		return ptr;
	}

	void setSize(int nSize) { // Set size
		size = nSize;
	}

	void setPtr(T* nPtr) { // Set ptr
		ptr = nPtr;
	}

	void extendMem(int nSize) { // Extend memory
		changeMemSize(nSize);
	}

	void shrinkMem(int nSize, int first) { // Shrink memory
		changeMemSize(nSize, first);
	}

	bool isNull() { // Return if pointer is null or not
		if(ptr == 0)
			return true;
		else
			return false;
	}
};

#endif
