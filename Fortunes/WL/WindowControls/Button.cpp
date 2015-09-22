#include <WL/WindowControls/Button.h>
#include <CL/Memory.h>

namespace WL
{
	namespace SubWindow
	{
		class _Button
		{
		public:
			Color bg;
			Label* title;
			_Button() { title = NULL; }
		};

		Button::~Button()
		{
			if(self) cl_delete(self->title);
			cl_delete(self);
		}
		st Button::init()
		{
			if(!BaseWindow::init()) return FALSE;

			self = cl_new(_Button);
			if(self == NULL) return FALSE;

			self->title = cl_new(Label);
			if(self->title == NULL) return FALSE;
			if(!self->title->init()) return FALSE;

			add_window(self->title);
			self->title->show();

			return TRUE;
		}

		Label* Button::get_title()
		{
			return self->title;
		}
		void Button::set_frame(const Rect* r) {
			Rect ir = *r;
			ir.x = 0;
			ir.y = 0;
			self->title->set_frame(&ir); 
			BaseWindow::set_frame(r);
		}
		void Button::set_frame(st x, st y, st w, st h) { self->title->set_frame(0, 0, w, h); BaseWindow::set_frame(x, y, w, h); }

		void Button::set_light(const Color* c)
		{
			if(c) set_background_color(c);
			else
			{
				Color tmp = self->bg;
				tmp.add_value(+50);
				BaseWindow::set_background_color(&tmp);
			}
		}
		void Button::set_dark(const Color* c)
		{
			if(c) set_background_color(c);
			else
			{
				Color tmp = self->bg;
				tmp.add_value(-50);
				BaseWindow::set_background_color(&tmp);
			}
		}
		void Button::set_normal(const Color* c)
		{
			if(c) set_background_color(c);
			else
				BaseWindow::set_background_color(&self->bg);
		}
		void Button::set_background_color(const Color* c)
		{
			self->bg = *c;
			BaseWindow::set_background_color(c);
		}
		void Button::set_background_color(st r, st g, st b)
		{
			self->bg.red = r; self->bg.green = g; self->bg.blue = b;
			BaseWindow::set_background_color(r, g, b);
		}

		st Button::event_for_touch(TouchEvent* e)
		{
			if(e->get_type() == WL::TouchEvent::Enter)
				set_light(NULL);
			else
			{
				if(e->get_type() == WL::TouchEvent::Leave)
					set_normal(NULL);
			}
			return TRUE;
		}
	}
}