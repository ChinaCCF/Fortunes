#include <windows.h>
#include <WL/Graphics/Geometry.h>
#include <WL/Graphics/Graphics.h>
#pragma once

namespace WL
{
	class IRender
	{
	public:
		virtual void set_color(ft a, st r, st g, st b) {}
		virtual void set_color(Color* c) {}

		virtual void set_pen_width(st width) {}

		virtual void fill_rect(Rect* r) {}
		virtual void draw_rect(Rect* r) {}

		virtual void fill_rect_with_corner(Rect* r, st corner) {}
		virtual void draw_rect_with_corner(Rect* r, st corner) {}

		virtual void fill_ellipse(Rect* r) {}
		virtual void draw_ellipse(Rect* r) {}

		virtual void set_clip(Rect* r) {}
		virtual void offset_x(st x) {}
		virtual void offset_y(st y) {}
		virtual void offset(st x, st y) {}

		static IRender* render_of_gdi(HDC hdc);
	};
}
