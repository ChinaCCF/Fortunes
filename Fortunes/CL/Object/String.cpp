#include <CL/Object/String.h>

namespace CL {
	void  StringUtil::string_copy(char* buf, st buf_size, const char* str)
	{
		st len = strlen(str);
		if(len >= buf_size) len = buf_size - 1;
		memcpy(buf, str, len + 1);
	}
}