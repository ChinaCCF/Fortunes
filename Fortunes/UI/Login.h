#include <WL/Window.h>
#pragma once

namespace Fortunes
{
	class _Login;
	class Login : public WL::Window
	{ 
		_Login* self;
	public:
		Login() { self = NULL; }
		~Login();
		st init();

		virtual void redraw(WL::IRender* render, WL::Rect* r);
	};
}