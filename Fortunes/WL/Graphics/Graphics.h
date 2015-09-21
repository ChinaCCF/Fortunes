#include <CL/CL.h>
#include <math.h>
#include <CL/Object/String.h>

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
		st red, green, blue;
		Color() { alpha = 1.0; red = 255; green = 255; blue = 255; }
		Color(st r, st g, st b) { alpha = 1.0; red = r; green = g; blue = b; }
		void set(st r, st g, st b) { red = r; green = g; blue = b; }
		void set(ft a, st r, st g, st b) { alpha = a; red = r; green = g; blue = b; }

		void add_value(st val)
		{
			red += val;
			blue += val;
			green += val;

			red = CL::MAX(CL::MIN(255, red), 0);
			green = CL::MAX(CL::MIN(255, green), 0);
			blue = CL::MAX(CL::MIN(255, blue), 0);
		}
		st operator==(const Color& c)
		{
			if(abs(alpha - c.alpha) >= 0.01) return FALSE;
			if(red != c.red) return FALSE;
			if(green != c.green) return FALSE;
			if(blue != c.blue) return FALSE;
			return TRUE;
		}

		st operator != (const Color& c) { return !this->operator==(c); }
	};

	class Font
	{
	public:
		char name[32];
		st size;
		st is_bold; 
		Font()
		{
			size = 15;
			is_bold = FALSE;
			CL::StringUtil::string_copy(name, 32, "ÐÂËÎÌå");
		}
		void set(st s, st b, const char* n) { size = s; is_bold = b; CL::StringUtil::string_copy(name, 32, n); }
	};
}
