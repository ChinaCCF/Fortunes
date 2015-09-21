#include <WL/Event.h>
#include <CL/Memory.h>

namespace WL
{
	/******************************************************/
	/******************************************************/
	class _KeyBoardEvent
	{
	public:
		s64 time;
		KeyBoardEvent::KeyType type;
		st val;
	};
	/******************************************************/
	KeyBoardEvent::KeyBoardEvent() { self = NULL; }
	KeyBoardEvent::~KeyBoardEvent() { cl_delete(self); }
	st KeyBoardEvent::init(KeyBoardEvent::KeyType type, st val) {
		self = cl_new(_KeyBoardEvent);
		if(self == NULL) return FALSE;
		self->type = type;
		self->val = val;
		return TRUE;
	}
	s64 KeyBoardEvent::get_time() { return self->time; }
	const KeyBoardEvent::KeyType KeyBoardEvent::get_type() { return self->type; }
	char KeyBoardEvent::get_char() { return (char)self->val; }
	const KeyBoardEvent::KeyValue KeyBoardEvent::get_key() { return (KeyValue)self->val; }


	/******************************************************/
	/******************************************************/
	class _TouchEvent
	{
	public:
		s64 time;
		TouchEvent::TouchType type;
		st count;
		Point* points;
	};
	/******************************************************/
	TouchEvent::TouchEvent() { self = NULL; }
	TouchEvent::~TouchEvent() { if(self) cl_free(self->points); cl_delete(self); }
	st TouchEvent::init(TouchEvent::TouchType type, st count, Point* points)
	{ 
		self = cl_new(_TouchEvent);
		if(self == NULL) return FALSE;
		self->type = type;
		self->count = count;
		self->points = cl_alloc_type_with_count(Point, count);
		memcpy(self->points, points, count * sizeof(Point));
		return TRUE;
	}
	s64 TouchEvent::get_time() { return self->time; }
	TouchEvent::TouchType TouchEvent::get_type() { return self->type; }
	st TouchEvent::get_count() { return self->count; }
	Point* TouchEvent::get_points() { return self->points; }
}