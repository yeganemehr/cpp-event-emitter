#ifndef EVENTEMITTER_HPP
#define EVENTEMITTER_HPP

#include <functional>
#include <vector>
#include <map>
#include "Event.hpp"

typedef std::function<void(const Event *event)> listener_cb_t;

class EventEmitter
{
public:
	static constexpr event_type_t ALL_EVENTS = 0xFFFFFFFF;
	void addListener(event_type_t type, const listener_cb_t &listener, bool once = false);
	void prependListener(event_type_t type, const listener_cb_t &listener, bool once = false);
	void prependOnceListener(event_type_t type, const listener_cb_t &listener);
	void removeAllListeners(event_type_t type);
	void removeAllListeners();
	inline void on(event_type_t type, const listener_cb_t &listener)
	{
		addListener(type, listener);
	}
	inline void once(event_type_t type, const listener_cb_t &listener)
	{
		addListener(type, listener, true);
	}
	inline void off(event_type_t type)
	{
		removeAllListeners(type);
	}
	bool emit(const Event *event);

private:
	struct EventListener
	{
		event_type_t event;
		listener_cb_t cb;
		bool once;
	};
	std::vector<EventListener> listeners;
};

#endif