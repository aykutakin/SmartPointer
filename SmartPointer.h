/****************************************************************************
* Developer: Aykut Akin  													*
*																			*
* The code is implemented under Windows 7 OS with Visual C++ 2010 Express	*
* The code also tested under Ubuntu 10.10 OS								*
*																			*
* Description: Smart pointer library for C++								*
*																			*
****************************************************************************/

#ifndef __SMART_POINTER_H__
#define __SMART_POINTER_H__

#include "SmartMemory.h"
#include <algorithm>
#include <map>
#include <vector>

static int nextId = 0; // ID counter
static std::map<long, std::vector<long>*> refInfo; // first: Memory address that will be referenced, second: All mem's addresses that references first address

template<class T>
class SmartPointer {
	template <class TT> friend class SmartPointer; // All SmartPointer's must be able to talk each other. Because we source's mem address.

private:

	int id; // Specific value for each pointer
	SmartMemory<T> mem; // Memory information
	bool isNew; // Is memory area allocated outside the SmartPointer

	void init(int nSize) { // Initializer
		id = nextId++;
		mem.init(nSize);
		isNew = true;
	}

	long getMemAddress() const { // Return mem address of pointer
		return (long) &mem;
	}

	void refDec(long memAddress) { // Delete memAddress from mem's references list
		std::vector<long>* refList = refInfo.find((long) mem.getPtr())->second;
		refList->erase(find(refList->begin(), refList->end(), memAddress)); // Erase memAddress from list
		if(refList->size() == 1) { // If only 1 pointer references that area, delete references informations from refInfo
			delete refList;
			refInfo.erase((long) mem.getPtr());
		}
	}

	template<class TT>
	void refInc(const SmartPointer<TT>& src) { // Insert mem address to Source reference list
		if(refInfo.find((long) src.getAddress()) == refInfo.end()) { // If there is no reference this area before
			refInfo.insert(std::pair<long,std::vector<long>*>((long) src.getAddress(), new std::vector<long>())); // Create reference list
			refInfo.find((long) src.getAddress())->second->push_back(src.getMemAddress()); // Add source as a reference
		}
		refInfo.find((long) src.getAddress())->second->push_back((long) &mem); // Add mem's address to source's reference list
	}

	void remove() {
		mem.freeMem(isNew); // Release mem if allocated in SmartPointer
		mem.setPtr(0);
		mem.setSize(0);
	}

	void reArrangeMem(long oldMem) {
		if(refInfo.find(oldMem) != refInfo.end()) { // If there is a reference to this area
			std::vector<long>* refList = refInfo.find(oldMem)->second;
			for(unsigned int i=0; i<refList->size(); i++) { // Change all the references informations
				SmartMemory<T>* memValue = (SmartMemory<T>*) refList->at(i);
				memValue->setSize(mem.getSize());
				memValue->setPtr(mem.getPtr());
			}
			refInfo.insert(std::pair<long,std::vector<long>*>((long) mem.getPtr(), refList)); // Insert new referenced address
			refInfo.erase(oldMem); // Erase old values
		}
	}

	void handleMem() {
		if(refInfo.find((long) mem.getPtr()) != refInfo.end()) // If there is a reference to mem's memory
			refDec((long) &mem); // Decrease reference
		else // If there is no reference to mem, delete unnecessary space
			remove();
	}

public:
	
	SmartPointer() { // Constructor
		init(0);
	}

	SmartPointer(int nSize) { // Constructor
		if(nSize < 0)
			throw "Size must be bigger than 0";
		init(nSize);
	}

	SmartPointer(const SmartPointer& src) {
		id = nextId++;
		std::memcpy(&mem,&src.mem,sizeof(SmartMemory<T>));
		refInc(src); // Increase reference to new source
		isNew = true;
	}

	SmartPointer(T* src, int size, bool isNewSrc = true) {
		id = nextId++;
		mem.setSize(size);
		mem.setPtr(src);
		isNew = isNewSrc;
	}
	
	~SmartPointer() { // Destructor
		handleMem();
	}

	T& operator*() const { // '*' operator: Returns pointer's value
		return *(mem.getPtr());
	}

	T* operator->() const { // '->' operator: Returns pointer's content
		return (mem.getPtr());
	}

	T& operator[](int i) const { // '[]' operator: Returns selected value
		if(i >= 0 && i < mem.getSize())
			return (mem.getPtr())[i];
		else
			throw "Unaccessible area";
	}

	T& operator+(int offset) const { // '+' operator: '[]' constraints abrogated
		return *(mem.getPtr() + offset);
	}

	T& operator=(SmartPointer& src) { // '=' operator: Set equal two SmartPointer
		if(mem.getPtr() != src.getAddress()) { // If they are not already equal
			handleMem(); // Handle old memory
			std::memcpy(&mem,&src.mem,sizeof(SmartMemory<T>));
			refInc(src); // Increase reference to new source
		}	
		return *(mem.getPtr());
	}

	void setSmartPtr(T* src, int size, bool isNewSrc = true) {
		if(size < 0)
			throw "Size must be bigger than 0";
		handleMem(); // Handle old memory
		mem.setSize(size);
		mem.setPtr(src);
		isNew = isNewSrc;
	}

	int getId() const {  // Returns pointer's ids
		return id;
	}

	int getSize() const { // Returns size
		return mem.getSize();
	}

	int getRefCount() const {
		if(refInfo.find((long) mem.getPtr()) == refInfo.end())
			return 0;
		else
			return refInfo.find((long) mem.getPtr())->second->size();
	}

	T* getAddress() const { // Returns pointed address
		return mem.getPtr();
	}

	bool getIsNew() const { // Returns isNew
		return isNew;
	}

	bool isNull() { // Returns pointer is null or not
		return mem.isNull();
	}

	void extend(int nSize) { // Extend the memory space of pointer
		if(nSize <= mem.getSize())
			throw "Values must be bigger than size";
		long oldMem = (long) mem.getPtr();
		mem.extendMem(nSize); // Extend memory space
		reArrangeMem(oldMem); // Handle with references
	}

	void shrink(int nSize, int first) { // Shrink the memory space of pointer
		if(nSize + first > mem.getSize())
			throw "Values total must be smaller than size";
		long oldMem = (long) mem.getPtr();
		mem.shrinkMem(nSize, first); // Shrink memory space
		reArrangeMem(oldMem); // Handle with references
	}

	void destroy() { // Delete memory space manually
		if(refInfo.find((long) mem.getPtr()) != refInfo.end()) { // If address has reference
			std::vector<long>* refList = refInfo.find((long) mem.getPtr())->second;
			refInfo.erase((long) mem.getPtr()); // Delete from refInfo
			for(unsigned int i=0; i<refList->size(); i++) { // Set null to all references
				SmartMemory<T>* memValue = (SmartMemory<T>*) refList->at(i);
				if(memValue == &mem)
					continue;
				memValue->setPtr(0);
				memValue->setSize(0);
			}
			delete refList;
		}
		remove(); // Delete allocated memory space
	}

	template <class TT>
	operator SmartPointer<TT>() {
		return SmartPointer<TT>(mem.getPtr(), mem.getSize(), false);
	}

	template <class TT>
	T& operator=(SmartPointer<TT>& src) { // '=' operator: Set equal two SmartPointer
		if(mem.getPtr() != src.getAddress()) { // If they are not already equal
			handleMem(); // Handle old memory
			std::memcpy(&mem,&src.mem,sizeof(SmartMemory<T>));
			refInc(src); // Increase reference to new source
		}
		return *(mem.getPtr());
	}

};

#endif
