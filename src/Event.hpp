#ifndef EVENT_HPP
#define EVENT_HPP

#include <stdint.h>

typedef uint32_t event_type_t;

class Event
{
public:
	virtual event_type_t getType() const = 0;
};

#endif