#include <WL/Graphics/Render.h>
#pragma once

namespace WL
{
	class IView
	{
	public:
		
		virtual void set_frame(const Rect* r) {}
		virtual void set_frame(st x, st y, st w, st h) {}
		virtual void get_frame(const Rect* r) {}

		virtual void set_background_color(const Color* c) {}
		virtual void set_background_color(st r, st g, st b) {}
		virtual void get_background_color(const Color* c) {}

		virtual void set_alpha(ft val) {}
		virtual ft get_alpha() { return 0; }

		virtual void show() {}
		virtual void hide() {}

		virtual void update() {}
		virtual void redraw(const IRender* render, const Rect* r) {}

		virtual void add_child(const IView* v) {}
		virtual void remove_child(const IView* v) {}
		virtual void remove_from_parent() {}
		virtual void bring_to_front() {}
	};
}