#include <CL/CL.h>

#ifndef __CL_Util_JSON__
#define __CL_Util_JSON__

namespace cl
{
	class _JSON;
	class JSON
	{
		_JSON* self;
	public:
		enum Type
		{
			String,
			Int,
			Float,
			Bool,
			Array,
			Object,
			Null
		};

		st init();
		void uninit();

		st parse(const char* str);
		const char* to_string();

		const char* value_of_string();
		st value_of_int();
		ft value_of_float();
		st value_of_bool();

		st size();
		JSON* operator[](st index);
		JSON* operator[](const char* name);
	};
}

#endif