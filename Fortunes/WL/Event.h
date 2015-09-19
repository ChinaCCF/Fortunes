#include <CL/CL.h>
#include <WL/Graphics/Geometry.h>
#pragma once

namespace WL
{
	class _Event;
	class Event
	{
		_Event* self;
	public:
		enum EventType
		{
			Touch,
			Keyboard
		};

		Event();
		~Event();
		st init(s64 time, EventType type);

		s64 time();
		EventType type();
	};

	class _KeyBoardEvent;
	class KeyBoardEvent : public Event
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
		KeyType get_key_type();
		char get_char();
		KeyValue get_key();
	};

	class _TouchEvent;
	class TouchEvent : public Event
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
		st count();
		Point* points();
	};

	class IResponse
	{
	public:
		virtual st event_for_keyboard(KeyBoardEvent* e) { return FALSE; }
		virtual st event_for_touch(TouchEvent* e) { return FALSE; }
		virtual void redraw() {}
	};
}