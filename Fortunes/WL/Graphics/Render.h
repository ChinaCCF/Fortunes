#include <windows.h>
#include <WL/Graphics/Geometry.h>
#include <WL/Graphics/Graphics.h>
#pragma once

namespace WL
{
	class IRender
	{
	public:
		cl_property(IRender, Color*, color); 
		cl_property(IRender, Font*, font);
		cl_property(IRender, st, line_width);
		//cl_property(IRender, Rect*, clip);
		//cl_property(IRender, Point*, origin);
		IRender() : cl_init_property(IRender, color),
			cl_init_property(IRender, font),
			cl_init_property(IRender, line_width)
		{}

		virtual void draw_line(Point* p1, Point* p2) {}
		virtual void draw_rect(const Rect* r) {}
		virtual void draw_rect(ft x, ft y, ft w, ft h) {}
		virtual void draw_rect_with_corner(const Rect* r, st corner) {}
		virtual void draw_ellipse(const Rect* r) {}
		virtual void draw_ellipse(ft x, ft y, ft w, ft h) {}
		virtual void draw_text(const Rect* r, const char* str) {}
		virtual void draw_image(ImageData* img, const Rect* r) {}

		virtual void fill_rect(const Rect* r) {}
		virtual void fill_rect(ft x, ft y, ft w, ft h) {}
		virtual void fill_rect_with_corner(const Rect* r, st corner) {}
		virtual void fill_ellipse(const Rect* r) {}
		virtual void fill_ellipse(ft x, ft y, ft w, ft h) {}
		virtual void fill_gradient_rect(const Rect* r, const Color* c1, const Color* c2, const Point* p1, const Point* p2) {}

		virtual ft text_height(const char* str) { return 0; }
		virtual ft text_width(const char* str) { return 0; } 

		/******************************************************/
		/******************************************************/
		static IRender* create_gdi_render(HDC hdc);
		static void destroy_render(IRender* render);
	};
}
