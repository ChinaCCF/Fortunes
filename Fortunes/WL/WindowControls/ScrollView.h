#include <WL/Window.h>
#pragma once

namespace WL
{
	namespace SubWindow
	{
		class _ScrollView;
		class ScrollView :public BaseWindow
		{
			_ScrollView* self;
		public:
			st init();
			void uninit();

			void set_content_size(WL::Size* size);
			st event_for_wheel(st direction);
			virtual void update();
		};
	}
}