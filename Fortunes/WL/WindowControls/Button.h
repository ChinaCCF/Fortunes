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

			st init();
			~BaseButton();

			void set_light(const Color* c);
			void set_dark(const Color* c);
			void set_normal(const Color* c);
			ButtonStatus get_status();

			void set_click(ButtonClick click, void* extra);

			//IView
			virtual void set_background_color(const Color* c);
			virtual void set_background_color(st r, st g, st b);
			//end of IView

			//IResponse
			virtual st event_for_touch(TouchEvent* e);
			//end of IResponse
		};

		class _Button;
		class Button :public BaseButton
		{
			_Button* self;
		public: 
			st init();
			~Button();
		
			Label* get_title();

			//IView
			virtual void set_frame(const Rect* r);
			virtual void set_frame(ft x, ft y, ft w, ft h);
			virtual void redraw(IRender* render);
			//end of Iview

			//IResponse
			virtual st event_for_touch(TouchEvent* e);
			//end of IResponse
		};
	}
}