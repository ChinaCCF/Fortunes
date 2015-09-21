#include <WL/Window.h>
#pragma once

namespace Fortunes
{
	class _Login;
	class Login : public WL::Window
	{ 
		_Login* self;
	public:
		st init();
	};
}