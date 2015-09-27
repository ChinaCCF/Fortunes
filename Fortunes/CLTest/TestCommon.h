#include <CL/Memory.h>
#pragma once 

#define CL_ASSERT(x) do{if(!(x)){__asm{int 3}}}while(FALSE)