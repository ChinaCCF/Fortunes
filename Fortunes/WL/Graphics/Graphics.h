#include <CL/CL.h>  
#include <Windows.h>
#include <CL/Memory.h>

#pragma once

namespace WL
{
	class Color
	{
	public:
		ft alpha = 1.0;
		st red = 255;
		st green = 255;
		st blue = 255;
		Color() = default;
		Color(st r, st g, st b) { alpha = 1.0; red = r; green = g; blue = b; }
		Color(ft a, st r, st g, st b) { alpha = a; red = r; green = g; blue = b; }
		st init() { return TRUE; }
		void set(st r, st g, st b) { red = r; green = g; blue = b; }
		void set(ft a, st r, st g, st b) { alpha = a; red = r; green = g; blue = b; }

		void add_value(st val)
		{
			red += val;
			blue += val;
			green += val;

			red = cl::MAX(cl::MIN(255, red), 0);
			green = cl::MAX(cl::MIN(255, green), 0);
			blue = cl::MAX(cl::MIN(255, blue), 0);
		}
		st operator==(const Color& c)
		{
			if(!cl::MemoryUtil::is_float_equ(alpha, c.alpha)) return FALSE;
			if(red != c.red) return FALSE;
			if(green != c.green) return FALSE;
			if(blue != c.blue) return FALSE;
			return TRUE;
		}

		st operator != (const Color& c) { return !this->operator==(c); }
	};

	class Font
	{
		char* name_ = NULL;
		HFONT font_ = NULL;
	public:
		st size = 15;
		st is_bold = FALSE;
		cl_define_property(Font, const char*, name);
		Font();
		Font(const Font&) = delete;
		Font& operator = (const Font&) = delete;
		~Font();
		st init();
		operator HFONT();
	};

	class ImageData
	{
		st has_chage_ = TRUE;
		char* file_ = NULL;
		void* data_ = NULL;
	public:
		ImageData();
		ImageData(const ImageData&) = delete;
		ImageData& operator = (const ImageData&) = delete;
		~ImageData(); 
		st init() { return TRUE; }
		cl_define_property(ImageData, const char*, file);
		cl_define_readonly_property(ImageData, st, width);
		cl_define_readonly_property(ImageData, st, height);
		cl_define_readonly_property(ImageData, void*, image);
	};
}
