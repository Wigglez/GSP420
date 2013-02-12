#include "MemoryManager.h"


MemoryManager::MemoryManager()  //constructor
{

}


MemoryManager::~MemoryManager()  //destructor
{
	 ;
}


Void* MemoryManager::AllocateMemory( uInt size )
{
	return malloc(size);
}

Void FreeMemory(Void* ptr)
{
	free(ptr);
}