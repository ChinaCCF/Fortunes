#include <CL/CL.h>
#pragma once

namespace WL
{
	class Color
	{
	public:
		enum
		{
			Color_Max_Value = 255
		};
		ft alpha;
		s16 red, green, blue;
		Color() { alpha = 1.0; red = 255; green = 255; blue = 255; }
		void set(s16 r, s16 g, s16 b) { red = r; green = g; blue = b; }
		void set(ft a, s16 r, s16 g, s16 b) { alpha = a; red = r; green = g; blue = b; }
	};

	//class Font
	//{
	//public:
	//	char* name_;
	//	u8 is_bold_;
	//	u8 size_;
	//public:
	//	explicit Font()
	//	{
	//		size_ = 15;
	//		is_bold_ = FALSE;
	//		name_ = cl_string_copy("ÐÂËÎÌå");
	//	}
	//	~Font() { free(name_); }
	//};
}
