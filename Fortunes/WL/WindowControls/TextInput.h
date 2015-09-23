#include <WL/Window.h>
#pragma once

namespace WL
{
	namespace SubWindow
	{
		class _TextInput;
		class TextInput :public BaseWindow
		{
			_TextInput* self;
		public:
			TextInput() { self = NULL; }
			~TextInput();
			st init();

			void set_text(const char* text);
			const char* get_text();
			st get_text_length();

			void set_font(Font* font);
			void get_font(Font* font);

			void set_is_password(st val);
			st get_is_password();

			void set_max_length(st len);

			virtual void set_focus();

			virtual st event_for_keyboard(WL::KeyBoardEvent* e);
		};
	}
}