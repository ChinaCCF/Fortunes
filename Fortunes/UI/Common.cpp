#include <UI/Common.h>

namespace Fortunes
{
	void CloseButton::redraw(WL::IRender* render)
	{
		WL::Color c;
		get_background_color(&c);
		render->set_color(&c);
		WL::Rect r;
		get_frame(&r);
		r.x = 0;
		r.y = 0;

		r.shrink(1);
		render->fill_ellipse(&r);

		c.set(250, 250, 250);
		render->set_color(&c);
		render->set_pen_width(2);

		r.expand(1);
		WL::Point center;
		center.set(r.w / 2, r.h / 2);
		ft len = cl::MIN(r.w, r.h);
		len /= 4;

		WL::Point p1, p2;
		p1 = center;
		p2 = center;

		p1.x -= len;
		p1.y -= len;

		p2.x += len;
		p2.y += len;

		render->draw_line(&p1, &p2);

		p1 = center;
		p2 = center;
		p1.x -= len;
		p1.y += len;

		p2.x += len;
		p2.y -= len;
		render->draw_line(&p1, &p2);
	}
}