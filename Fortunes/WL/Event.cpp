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
	st KeyBoardEvent::init()
	{
		self = cl_alloc_type(_KeyBoardEvent);
		if(self == NULL) return FALSE;
		return TRUE;
	}	
	KeyBoardEvent::~KeyBoardEvent() { cl_free(self); }

	void KeyBoardEvent::set(KeyBoardEvent::KeyType type, st val)
	{
		self->type = type;
		self->val = val;
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
	st TouchEvent::init()
	{
		self = cl_alloc_type(_TouchEvent);
		if(self == NULL) return FALSE;
		return TRUE;
	}
	TouchEvent::~TouchEvent() { 
		if(self) cl_free(self->points);
		cl_free(self); 
	}
	
	st TouchEvent::set(TouchEvent::TouchType type, st count, Point* points)
	{
		self->type = type;
		self->count = count;
		self->points = cl_alloc_type_with_count(Point, count);
		if(self->points == NULL) return FALSE;
		cl::MemoryUtil::copy(self->points, points, count * sizeof(Point));
		return TRUE;
	}
	s64 TouchEvent::get_time() { return self->time; }
	TouchEvent::TouchType TouchEvent::get_type() { return self->type; }
	st TouchEvent::get_count() { return self->count; }
	Point* TouchEvent::get_points() { return self->points; }
}