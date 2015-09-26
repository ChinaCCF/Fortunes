#include <WL/WindowControls/Button.h>

namespace Fortunes
{
	class CloseButton : public WL::SubWindow::BaseButton
	{
	public:
		virtual void redraw(WL::IRender* render);
	};
}