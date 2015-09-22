#include <CL/CL.h>
#include <math.h>
#include <CL/Object/String.h>
#include <Windows.h>
#include <CL/Memory.h>

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

	class _Font;
	class Font
	{
		_Font* self;
	public: 
		Font() { self = NULL; }
		~Font(); 
		st init();

		void set_size(st val);
		st get_size() const;

		void set_is_bold(st val);
		st get_is_bold() const;

		void set_name(const char* name);
		const char* get_name() const;

		void* get_HFONT();

		Font* copy();
		Font& operator = (const Font&);
	};

	class _ImageData;
	class ImageData
	{
		_ImageData* self;
	public:
		ImageData() { self = NULL; }
		~ImageData();
		st init();
		 
		void set_file(const char* file);
		const char* get_file();

		st get_width();
		st get_height();

		void* get_image();//Gdiplus::Image
	};
}
