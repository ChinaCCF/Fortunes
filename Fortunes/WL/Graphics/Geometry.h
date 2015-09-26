#include <CL/CL.h>
#pragma once

namespace WL
{
	class Point
	{
	public:
		ft x, y;
		Point() { x = 0; y = 0; }
		void set(ft _x, ft _y) { x = _x; y = _y; }
	};

	class Size
	{
	public:
		ft w, h;
		Size() { w = 0; h = 0; }
		Size(ft _w, ft _h) { w = _w; h = _h; }
		void set(ft _w, ft _h) { w = _w; h = _h; }

		void fit_in(const Size* size)
		{
			if(size->w == 0 || size->h == 0) return;

			if((w / h) <= (size->w / size->h))
			{
				w = size->w;
				h = w * (size->h / size->w);
			}
			else
			{
				h = size->h;
				w = h * (size->w / size->h);
			}
		}
	};

	class Rect : public Point, public Size
	{
	public:
		void set(ft x, ft y, ft w, ft h) { Point::set(x, y); Size::set(w, h); }

		st operator ==(const Rect& r)
		{
			if(x != r.x)  return FALSE;
			if(y != r.y)  return FALSE;
			if(w != r.w)  return FALSE;
			if(h != r.h)  return FALSE;
			return TRUE;
		}
		st operator !=(const Rect& r) { return !this->operator==(r); }

		void move_to_horizontal_center_in(const Rect* rect) { x = (rect->w - w) / 2 + rect->x; }
		void move_to_vertical_center_in(const Rect* rect) { y = (rect->h - h) / 2 + rect->y; }
		void move_to_right_in(const Rect* rect) { x = rect->w - w + rect->x; }
		void move_to_bottom_in(const Rect* rect) { y = rect->h - h + rect->y; }

		void move_to_center_in(const Rect* rect)
		{
			this->move_to_vertical_center_in(rect);
			this->move_to_horizontal_center_in(rect);
		}
		void move_to_bottom_right_in(const Rect* rect)
		{
			this->move_to_bottom_in(rect);
			this->move_to_right_in(rect);
		}

		void fit_in(const Rect* r)
		{
			Size si(w, h);
			Size so(r->w, r->h);
			si.fit_in(&so);
			w = si.w; h = si.h;
		}

		st test_point(const Point* p)
		{
			if(p->x < 0 || p->y < 0 || p->x > w || p->y > h)
				return FALSE;
			return TRUE;
		}
		void shrink(st val)
		{
			x += val;
			y += val;
			w -= 2 * val;
			h -= 2 * val;
		}
		void expand(st val)
		{
			x -= val;
			y -= val;
			w += 2 * val;
			h += 2 * val;
		}
		/**************************************************/
		st is_intersect_with(const Rect* rect)
		{
			if(rect->x >= (x + w)) return FALSE;
			if(rect->y >= (y + h)) return FALSE;
			if((rect->x + rect->w) <= x) return FALSE;
			if((rect->y + rect->h) <= y) return FALSE;
			return TRUE;
		}
		Rect get_intersect(const Rect* rect)
		{
			Rect r;
			if(is_intersect_with(rect))
			{
				r.x = cl::MAX(rect->x, x);
				r.y = cl::MAX(rect->y, y);

				ft right = cl::MIN(rect->x + rect->w, x + w);
				r.w = right - r.x;

				ft bottom = cl::MIN(rect->y + rect->h, y + h);
				r.h = bottom - r.y;
			}
			return r;
		}
		Rect get_merge(const Rect* rect)
		{
			Rect r;
			if(rect->w != 0 && rect->h != 0 && w != 0 && h != 0)
			{
				r.x = cl::MIN(x, rect->x);
				r.y = cl::MIN(y, rect->y);
				r.w = cl::MAX(x + w, rect->x + rect->w) - r.x;
				r.h = cl::MAX(y + h, rect->y + rect->h) - r.y;
			}
			return r;
		}
	};
}
