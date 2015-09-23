#include <WL/Window.h>
#include <WL/WindowControls/Label.h>
#pragma once

namespace WL
{
	namespace SubWindow
	{
		class BaseButton;
		typedef void(*ButtonClick)(BaseButton* btn, void* extra);

		class _BaseButton;
		class BaseButton : public BaseWindow
		{
			_BaseButton* self;
		public:
			enum ButtonStatus
			{
				Light,
				Dark,
				Normal
			};

			BaseButton() { self = NULL; }
			~BaseButton();
			st init();
 
			void set_light(const Color* c);
			void set_dark(const Color* c);
			void set_normal(const Color* c);
			ButtonStatus get_status();

			void set_click(ButtonClick click, void* extra);

			virtual void set_background_color(const Color* c);
			virtual void set_background_color(st r, st g, st b);

			virtual st event_for_touch(TouchEvent* e);
		};

		class _Button;
		class Button :public BaseButton
		{
			_Button* self;
		public:
			Button() { self = NULL; }
			~Button();
			st init();
 
			Label* get_title();

			void set_frame(const Rect* r);
			void set_frame(st x, st y, st w, st h);
		};
	}
}