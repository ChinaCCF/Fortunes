#include <WL/WindowControls/TextInput.h>
#include <CL/Memory.h>

namespace WL
{
	namespace SubWindow
	{
		class _TextInput
		{
		public:
			char* text;
			Font* font;
			Color color;
			st is_password;
			_TextInput() { text = NULL; is_password = FALSE; }
		};

		TextInput::~TextInput()
		{
			cl_free(self->text);
			cl_delete(self->font);
			cl_delete(self);
		}

		st TextInput::init()
		{
			if(!BaseWindow::init("edit", ES_AUTOHSCROLL)) return FALSE; //ES_PASSWORD
			self = cl_new(_TextInput);
			if(self == NULL) return FALSE;
			self->font = cl_new(Font);
			if(self->font == NULL) return FALSE;
			if(!self->font->init()) return FALSE;
			self->font->set_size(20);
			SendMessage(BaseWindow::get_handle(), WM_SETFONT, (WPARAM)self->font->get_HFONT(), (LPARAM)MAKELONG(TRUE, 0));
			return TRUE;
		}
		void TextInput::set_text(const char* text)
		{
			if(text)			SetWindowTextA(BaseWindow::get_handle(), text);
		}
		const char* TextInput::get_text()
		{
			cl_free(self->text);

			HWND hwnd = BaseWindow::get_handle();
			st len = GetWindowTextLengthA(hwnd);
			self->text = cl_alloc_type_with_count(char, len + 1);
			GetWindowTextA(hwnd, self->text, len + 1);
			return self->text;
		}

		void TextInput::set_font(Font* font)
		{
			cl_delete(self->font);
			self->font = font->copy();
			update();
		}
		void TextInput::get_font(Font* font) { *font = *(self->font); }

		void TextInput::set_is_password(st val)
		{
			self->is_password = val;
			HWND hwnd = BaseWindow::get_handle();
			char c = 0;
			if(val)
				c = '*';
			SendMessage(hwnd, EM_SETPASSWORDCHAR, (WPARAM)c, 0);
		}
		st TextInput::get_is_password() { return self->is_password; }

		void TextInput::set_focus()
		{
			BaseWindow::set_focus();
			HWND hwnd = BaseWindow::get_handle();
			st len = GetWindowTextLengthA(hwnd);
			SendMessage(hwnd, EM_SETSEL, len, len);
		}

		st TextInput::event_for_keyboard(WL::KeyBoardEvent* e)
		{
			char buf[16];
			CL::StringUtil::format(buf, 16, "%c\n", e->get_char());
			cl_printf(buf);
			return 0;
		}
	}
}