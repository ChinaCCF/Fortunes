#include <WL/Event.h>
#include <CL/Memory.h>

namespace WL
{
	/******************************************************/
	/******************************************************/
	class _KeyBoardEvent
	{
	public:
		s64 time_;
		KeyBoardEvent::KeyType type_;
		st val_;
	};
	/******************************************************/
	KeyBoardEvent::KeyBoardEvent() { self = NULL; }
	KeyBoardEvent::~KeyBoardEvent() { cl_delete(self); }
	st KeyBoardEvent::init(KeyBoardEvent::KeyType type, st val) {
		self = cl_new(_KeyBoardEvent);
		if(self == NULL) return FALSE;
		self->type_ = type;
		self->val_ = val;
		return TRUE;
	}
	s64 KeyBoardEvent::get_time() { return self->time_; }
	KeyBoardEvent::KeyType KeyBoardEvent::get_type() { return self->type_; }
	char KeyBoardEvent::get_char() { return (char)self->val_; }
	KeyBoardEvent::KeyValue KeyBoardEvent::get_key() { return (KeyValue)self->val_; }


	/******************************************************/
	/******************************************************/
	class _TouchEvent
	{
	public:
		s64 time_;
		TouchEvent::TouchType type_;
		st count_;
		Point* points_;
	};
	/******************************************************/
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
	s64 TouchEvent::get_time() { return self->time_; }
	TouchEvent::TouchType TouchEvent::get_type() { return self->type_; }
	st TouchEvent::get_count() { return self->count_; }
	Point* TouchEvent::get_points() { return self->points_; }
}