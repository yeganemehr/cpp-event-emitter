#include "EventEmitter.hpp"

void EventEmitter::addListener(event_type_t type, const listener_cb_t &listener, bool once)
{
	listeners.push_back({type, listener, once});
}

void EventEmitter::prependListener(event_type_t type, const listener_cb_t &listener, bool once)
{
	listeners.insert(listeners.begin(), {type, listener, once});
}

void EventEmitter::prependOnceListener(event_type_t type, const listener_cb_t &listener)
{
	prependListener(type, listener, true);
}

void EventEmitter::removeAllListeners(event_type_t type)
{
	for (auto it = listeners.begin(); it != listeners.end();)
	{
		if (it->event == type)
		{
			listeners.erase(it);
			continue;
		}
		++it;
	}
}
void EventEmitter::removeAllListeners()
{
	listeners.clear();
}

bool EventEmitter::emit(const Event *event)
{
	bool hasListener = false;
	auto type = event->getType();
	for (auto it = listeners.begin(); it != listeners.end();)
	{
		if (it->event == type || it->event == EventEmitter::ALL_EVENTS)
		{
			hasListener = true;
			it->cb(event);
			if (it->once)
			{
				listeners.erase(it);
				continue;
			}
		}
		++it;
	}

	return hasListener;
}