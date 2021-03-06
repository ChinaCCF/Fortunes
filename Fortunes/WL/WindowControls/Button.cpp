#include <WL/WindowControls/Button.h>
#include <CL/Memory.h>
#include <WL/Graphics/Geometry.h>

namespace WL
{
	namespace SubWindow
	{
		class _BaseButton
		{
		public:
			Color bg;
			st is_down;
			BaseButton::ButtonStatus status;

			ButtonClick click;
			void* extra;
			_BaseButton() { is_down = FALSE;  extra = NULL; click = NULL; status = BaseButton::Normal; }
		};

		BaseButton::~BaseButton()
		{
			cl_delete(self);
		}
		st BaseButton::init()
		{
			if(!BaseWindow::init()) return FALSE;

			self = cl_new(_BaseButton);
			if(self == NULL) return FALSE;
			return TRUE;
		}
		void BaseButton::set_light(const Color* c)
		{
			self->status = Light;
			if(c) set_background_color(c);
			else
			{
				Color tmp = self->bg;
				tmp.add_value(+30);
				BaseWindow::set_background_color(&tmp);
			}
		}
		void BaseButton::set_dark(const Color* c)
		{
			self->status = Dark;
			if(c) set_background_color(c);
			else
			{
				Color tmp = self->bg;
				tmp.add_value(-30);
				BaseWindow::set_background_color(&tmp);
			}
		}
		void BaseButton::set_normal(const Color* c)
		{
			self->status = Normal;
			if(c) set_background_color(c);
			else
				BaseWindow::set_background_color(&self->bg);
		}
		BaseButton::ButtonStatus BaseButton::get_status()
		{
			return self->status;
		}

		void BaseButton::set_background_color(const Color* c)
		{
			self->bg = *c;
			BaseWindow::set_background_color(c);
		}
		void BaseButton::set_background_color(st r, st g, st b)
		{
			self->bg.red = r; self->bg.green = g; self->bg.blue = b;
			BaseWindow::set_background_color(r, g, b);
		}

		void BaseButton::set_click(ButtonClick click, void* extra)
		{
			self->click = click;
			self->extra = extra;
		}
		st BaseButton::event_for_touch(TouchEvent* e)
		{
			if(e->get_type() == WL::TouchEvent::Enter)
			{
				if(!self->is_down)
					set_light(NULL);
				else
					set_dark(NULL);
			}
			else
			{
				if(e->get_type() == WL::TouchEvent::Leave)
					set_normal(NULL);
				else
				{
					if(e->get_type() == WL::TouchEvent::Down)
					{
						self->is_down = TRUE;
						set_dark(NULL);
						SetCapture(BaseWindow::get_handle());
					}
					else
					{
						if(e->get_type() == WL::TouchEvent::Up)
						{
							st need = FALSE;
							if(self->is_down)
							{
								WL::Rect r;
								BaseWindow::get_frame(&r);
								if(r.test_point(e->get_points()))
									need = TRUE;
							}
							self->is_down = FALSE;
							set_normal(NULL);
							ReleaseCapture();
							if(need && self->click)
								self->click(this, self->extra);
						}
						else
						{
							if(e->get_type() == WL::TouchEvent::Move)
							{
								WL::Rect r;
								BaseWindow::get_frame(&r);
								if(r.test_point(e->get_points()))
								{
									if(!self->is_down)
										set_light(NULL);
									else
										set_dark(NULL);
								}
								else
									set_normal(NULL);
							}
						}
					}
				}
			}
			return TRUE;
		}

		class _Button
		{
		public:
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
			if(!BaseButton::init()) return FALSE;
			self = cl_new(_Button);
			if(self == NULL) return FALSE;

			self->title = cl_new(Label);
			if(self->title == NULL) return FALSE;
			if(!self->title->init()) return FALSE;

			add_window(self->title);
			self->title->show();
			return TRUE;
		}
		Label* Button::get_title() { return self->title; }
		void Button::set_frame(const Rect* r)
		{
			Rect ir = *r;
			ir.x = 0;
			ir.y = 0;
			self->title->set_frame(&ir);
			BaseWindow::set_frame(r);
		}
		void Button::set_frame(ft x, ft y, ft w, ft h) { self->title->set_frame(0, 0, w, h); BaseWindow::set_frame(x, y, w, h); }

	}
}