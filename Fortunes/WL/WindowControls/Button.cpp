#include <WL/WindowControls/Button.h>
#include <CL/Memory.h>

namespace WL
{
	namespace SubWindow
	{
		class _Button
		{
		public:
			enum
			{
				Title_Size = 64
			};
			char title[Title_Size];
			Font font;
			Color bg;
			Color font_color;
			_Button() { CL::StringUtil::string_copy(title, Title_Size, "title"); }
		};

		st Button::init()
		{
			if(!BaseWindow::init()) return FALSE;
			self = cl_new(_Button);
			if(self == NULL) return FALSE;

			return TRUE;
		}

		void Button::set_title(const char* title)
		{
			CL::StringUtil::string_copy(self->title, _Button::Title_Size, title);
			update();
		}
		const char* Button::get_title()
		{
			return self->title;
		}

		void Button::set_font(const Font* font)
		{
			self->font = *font;
			update();
		}
		void Button::get_font(Font* font)
		{
			*font = self->font;
		}
		void Button::set_title_color(const Color* c)
		{
			self->font_color = *c;
			update();
		}
		void Button::get_title_color(Color* c)
		{
			*c = self->font_color;
		}

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

		void Button::redraw(IRender* render, Rect* r)
		{
			BaseWindow::redraw(render, r);
			if(strlen(self->title) > 0)
			{
				render->set_font(&self->font);
				render->set_color(&self->font_color);

				WL::Size size;
				render->text_size(self->title, &size);

				Rect target;
				target.set(0, 0, size.w, size.h);
				target.move_to_center_in(r);

				render->draw_text(&target, self->title);
			}
		}
	}
}