#include <WL/WindowControls/ScrollView.h>

namespace WL
{
	namespace SubWindow
	{
		class _ScrollView
		{
		public:
			ft content_height;
			ft content_offset;
			ft content_move_distance;
			ft content_old_offset;
		};

		st ScrollView::init()
		{
			if(!BaseWindow::init()) return FALSE;
			self = cl_alloc_type(_ScrollView);
			if(self == NULL) return FALSE;
			self->content_height = 0;
			self->content_offset = 0;
			self->content_old_offset = 0;
			self->content_move_distance = 50;
			return TRUE;
		}
		void ScrollView::uninit()
		{
			cl_free(self);
		}

		void ScrollView::set_content_size(WL::Size* size)
		{
			self->content_height = size->h;
			update();
		}
		void ScrollView::update()
		{
			WL::Rect self_frame;
			get_frame(&self_frame);

			if(self->content_height >= self_frame.h)
			{
				if(self->content_offset > 0)
				{
					if(self->content_offset + self_frame.h > self->content_height)
						self->content_offset = self->content_height - self_frame.h;
				}
				else
					self->content_offset = 0;
			}
			else
			{
				self->content_height = self_frame.h;
				self->content_offset = 0;
			}

			if(abs(self->content_offset - self->content_old_offset) > 0.01)
				ScrollWindowEx(get_handle(), 0, (st)(self->content_old_offset - self->content_offset), NULL, NULL, NULL, NULL, SW_ERASE | SW_INVALIDATE | SW_SCROLLCHILDREN);
			self->content_old_offset = self->content_offset;
			UpdateWindow(get_handle());
		}
		st ScrollView::event_for_wheel(st direction)
		{
			if(abs(self->content_offset) < 0.1 && direction > 0)
				return TRUE;
			self->content_offset -= direction * self->content_move_distance;
			update();
			return TRUE;
		}
	}
}