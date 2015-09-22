#include <WL/Window.h>
#include <WL/WindowControls/Label.h>
#pragma once

namespace WL
{
	namespace SubWindow
	{
		class _Button;
		class Button :public BaseWindow
		{
			_Button* self;
		public:
			Button() { self = NULL; }
			~Button();
			st init();
 
			Label* get_title();
			virtual void set_frame(const Rect* r);
			virtual void set_frame(st x, st y, st w, st h);

			void set_light(const Color* c);
			void set_dark(const Color* c);
			void set_normal(const Color* c);
			virtual void set_background_color(const Color* c);
			virtual void set_background_color(st r, st g, st b);

			virtual st event_for_touch(TouchEvent* e); 
		};
	}
}