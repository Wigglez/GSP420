#include "Typedefs.h"
//#include <stdlib.h>
//#include <stdio.h>
#include <malloc.h>


class MemoryManager
{

private:

protected:

public:

    MemoryManager();

    ~MemoryManager();

    Void* AllocateMemory( uInt size );
        
    Void FreeMemory( Void* ptr );
        
};
