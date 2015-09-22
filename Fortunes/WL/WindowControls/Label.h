#include <WL/Window.h>
#pragma once

namespace WL
{
	namespace SubWindow
	{
		class _Label;
		class Label :public BaseWindow
		{
			_Label* self;
		public:
			Label() { self = NULL; }
			~Label();
			st init();

			void set_text(const char* text);
			const char* get_text();

			void set_font(const Font* font);
			void get_font(Font* font);

			void set_text_color(const Color* c);
			void get_text_color(Color* c); 
 
			virtual void redraw(IRender* render, Rect* r);
		};
	}
}