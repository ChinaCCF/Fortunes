#include <CL/CL.h>
#include <string.h>

#ifndef __CL_Object_String__
#define __CL_Object_String__

namespace CL
{
	class StringUtil
	{
	public:
		static void string_copy(char* buf, st buf_size, const char* str);
	};

 

	/*class String : public virtual Object
	{
	char* buffer_;
	char* string_;
	ut size_;
	ut length_;
	public:
	String()
	{
	buffer_ = NULL;
	string_ = NULL;
	size_ = 0;
	length_ = 0;
	}
	~String()
	{

	}
	ut length() { return length_; }
	const char* string() { return string_; }
	};*/
}

#endif