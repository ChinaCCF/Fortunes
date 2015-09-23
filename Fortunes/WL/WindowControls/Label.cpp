#include <WL/WindowControls/Label.h>
#include <CL/Memory.h>

namespace WL
{
	namespace SubWindow
	{
		class _Label
		{
		public:
			char* text;
			Font font;
			Color color;

			_Label() { text = NULL; }
		};

		Label::~Label()
		{
			cl_free(self->text);
			cl_delete(self);
		}

		st Label::init()
		{
			if(!BaseWindow::init()) return FALSE;
			self = cl_new(_Label);
			if(self == NULL)
				return FALSE;
			if(!self->font.init()) return FALSE;
			return TRUE;
		}
		void Label::set_text(const char* text)
		{
			if(text)
			{
				cl_free(self->text);
				self->text = cl_string_alloc(text);
				update();
			} 
		}
		const char* Label::get_text() { return self->text; }

		void Label::set_font(const Font* font)
		{
			self->font = *font;
			update();
		}
		void Label::get_font(Font* font) { *font = self->font; }

		void Label::set_text_color(const Color* c)
		{
			self->color = *c;
			update();
		}
		void Label::get_text_color(Color* c) { *c = self->color; }

		void Label::redraw(IRender* render, Rect* r)
		{
			if(self->text && (self->text) > 0)
			{
				render->set_font(&self->font);
				render->set_color(&self->color);

				WL::Size size;
				render->text_size(self->text, &size);

				Rect target;
				target.set(0, 0, size.w, size.h);

				st align = get_horizontal_align();
				if(align == 0)
					target.move_to_horizontal_center_in(r);
				else
				{
					if(align == 1)
						target.move_to_right_in(r);
				}

				align = get_vertical_align();

				if(align == 0)
					target.move_to_vertical_center_in(r);
				else
				{
					if(align == 1)
						target.move_to_bottom_in(r);
				}
				target.y += target.h * 0.1;
				render->draw_text(&target, self->text);
			}
		}
	}
}