#include <WL/Event.h>
#include <CL/Memory.h>

namespace WL
{
	class _Event
	{
	public:
		s64 time_;
		Event::EventType type_;
	};
	Event::Event() { self = NULL; }
	Event::~Event() { cl_delete(self); }
	st Event::init(s64 time, EventType type)
	{
		self = cl_new(_Event);
		if(self == NULL) return FALSE;
		self->time_ = time;
		self->type_ = type;
		return TRUE;
	}
	s64 Event::time() { return self->time_; }
	Event::EventType Event::type() { return self->type_; }


	class _KeyBoardEvent
	{
	public:
		KeyBoardEvent::KeyType type_;
		st val_;
	};

	KeyBoardEvent::KeyBoardEvent() { self = NULL; }
	KeyBoardEvent::~KeyBoardEvent() { cl_delete(self); }
	st KeyBoardEvent::init(KeyBoardEvent::KeyType type, st val) {
		self = cl_new(_KeyBoardEvent);
		if(self == NULL) return FALSE;
		self->type_ = type;
		self->val_ = val;
		return TRUE;
	}
	KeyBoardEvent::KeyType KeyBoardEvent::get_key_type() { return self->type_; }
	char KeyBoardEvent::get_char() { return (char)self->val_; }
	KeyBoardEvent::KeyValue KeyBoardEvent::get_key() { return (KeyValue)self->val_; }


	class _TouchEvent
	{
	public:
		TouchEvent::TouchType type_;
		st count_;
		Point* points_;
	};
	TouchEvent::TouchEvent() { self = NULL; }
	TouchEvent::~TouchEvent() { if(self) cl_free(self->points_); cl_delete(self); }
	st TouchEvent::init(TouchEvent::TouchType type, st count, Point* points)
	{
		self = cl_new(_TouchEvent);
		if(self == NULL) return FALSE;
		self->type_ = type;
		self->count_ = count;
		self->points_ = cl_alloc_type_with_count(Point, count);
		memcpy(self->points_, points, count * sizeof(Point));
		return TRUE;
	}
	st TouchEvent::count() { return self->count_; }
	Point* TouchEvent::points() { return self->points_; }
}