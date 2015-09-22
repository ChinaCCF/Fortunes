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

			void set_font(Font* font);
			void get_font(Font* font);

			void set_is_password(st val);
			st get_is_password();

			virtual void set_focus();

			virtual st event_for_keyboard(WL::KeyBoardEvent* e);
		};
	}
}