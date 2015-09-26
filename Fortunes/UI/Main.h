#include <WL/Window.h>
#pragma once

namespace Fortunes
{
	class _Main;
	class Main : public WL::Window
	{
		_Main* self;
	public:
		Main() { self = NULL; }
		~Main();
		st init();

		virtual void redraw(WL::IRender* render);
	};
}