#include <WL/WindowControls/ScrollView.h>

namespace WL
{
	namespace SubWindow
	{
		class _ScrollView
		{
		public:
			int i;
		};

		ScrollView::~ScrollView()
		{
			cl_delete(self);
		}
		st ScrollView::init()
		{
			if(!BaseWindow::init()) return FALSE; 
			self = cl_new(_ScrollView);
			if(self == NULL) return FALSE;
			
			return TRUE;

		}
		void ScrollView::set_content_size(WL::Size* size)
		{

		}
		st ScrollView::event_for_wheel(st direction)
		{
			return 0;
		}
	}
}