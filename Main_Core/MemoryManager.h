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

// I am still deciding what return should be used for the Memory Management


// Below is coding for the source code.  
// I apologize that there isn't much here, but commented out examples and just the constructor and destructor

//#include "Typedef.h"
////#include <stdlib.h>
////#include <stdio.h>
//#include <malloc.h>
//#include "MemoryManager.h"



//MemoryManager()  //constructor
//  :str(NULL),
//    size(0)
//{
//
//}
//
//
//~MemoryManager();  //destructor
//{
//	delete [] 
//};





// The coding below are just examples I was looking over while reserching memory management
/*

int main()
	{
		int *ptr_one;

		ptr_one = (int *)malloc(sizeof(int));

		if (ptr_one == 0)
		{
			printf("ERROR: Out of memory\n");
			return 1;
		}

		*ptr_one = 25;
		printf("%d\n", *ptr_one);

		free(ptr_one);

		return 0;
	}

*/


/*

void main()
{
	uInt size = 0;
	Void* ptr;

	MemoryManager mem;
	mem.AllocateMemory(size);
	mem.FreeMemory(ptr);
	mem.~MemoryManager();

}

*/
