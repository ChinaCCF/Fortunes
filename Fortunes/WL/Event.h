#include <CL/CL.h>
#include <WL/Graphics/Geometry.h>
#include <WL/Graphics/Render.h>

#pragma once

namespace WL
{
	 
	class _KeyBoardEvent;
	class KeyBoardEvent
	{
		_KeyBoardEvent* self;
	public:
		enum KeyValue
		{
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			Esc,
			Home,
			End,
			Insert,
			Delete,
			Tab,
			CapsLock,
			Shift,
			Ctrl,
			Alt,
			Backspace,
			Enter,
			PageUp,
			PageDown,
			Left,
			Right,
			Up,
			Down
		};
		enum KeyType
		{
			Char,
			Key
		};

		KeyBoardEvent();
		~KeyBoardEvent();
		st init(KeyType type, st val);

		s64 get_time();
		KeyType get_type();
		char get_char();
		KeyValue get_key();
	};

	class _TouchEvent;
	class TouchEvent
	{
		_TouchEvent* self;
	public:
		enum TouchType
		{
			Down,
			Up,
			Move
		};
		TouchEvent();
		~TouchEvent();
		st init(TouchType type, st count, Point* points);

		s64 get_time();
		TouchType get_type();
		st get_count();
		Point* get_points();
	};

	class IResponse
	{
	public:
		virtual st event_for_keyboard(KeyBoardEvent* e) { return FALSE; }
		virtual st event_for_touch(TouchEvent* e) { return FALSE; }
		virtual void redraw(IRender* render, Rect* r) {}
	};
}