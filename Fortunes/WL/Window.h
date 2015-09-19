#include <CL/CL.h>
#include <WL/Event.h> 
#pragma once

namespace WL
{
	class _Window;
	class Window : public IResponse
	{
		_Window* self;
	public:
		Window();
		~Window();
		st init();

		virtual st event_for_keyboard(KeyBoardEvent* e);
		virtual st event_for_touch(TouchEvent* e);
	};
}