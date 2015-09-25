#include <windows.h>
#include <WL/Graphics/Geometry.h>
#include <WL/Graphics/Graphics.h>
#pragma once

namespace WL
{
	class IRender
	{
	public:
		virtual void set_color(st r, st g, st b) {}
		virtual void set_color(ft a, st r, st g, st b) {}
		virtual void set_color(const Color* c) {}

		virtual void set_pen_width(st width) {}

		virtual void draw_line(Point* p1, Point* p2) {}

		virtual void fill_rect(const Rect* r) {}
		virtual void fill_rect(ft x, ft y, ft w, ft h) {}
		virtual void draw_rect(const Rect* r) {}

		virtual void fill_rect_with_corner(const Rect* r, st corner) {}
		virtual void draw_rect_with_corner(const Rect* r, st corner) {}

		virtual void fill_ellipse(const Rect* r) {}
		virtual void fill_ellipse(ft x, ft y, ft w, ft h) {}
		virtual void draw_ellipse(const Rect* r) {}

		virtual void set_clip(const Rect* r) {}
		virtual void offset_x(st x) {}
		virtual void offset_y(st y) {}
		virtual void offset(st x, st y) {}

		virtual void set_font(Font* font) {}
		virtual ft text_height(const char* str) { return 0; }
		virtual ft text_width(const char* str) { return 0; }
		virtual void text_size(const char* str, Size* size) {}
		virtual void draw_text(const Rect* r, const char* str) {}

		virtual void draw_image(ImageData* img, const Rect* r) {}

		virtual void fill_gradient_rect(const Rect* r, const Color* c1, const Color* c2, const Point* p1, const Point* p2) {}

		static IRender* create_gdi_render(HDC hdc);
	};
}
