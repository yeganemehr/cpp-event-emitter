[![PlatformIO Registry](https://badges.registry.platformio.org/packages/yeganemehr/library/event-emitter.svg)](https://registry.platformio.org/libraries/yeganemehr/event-emitter)
[![Build Examples](https://github.com/yeganemehr/cpp-event-emitter/actions/workflows/build-examples.yml/badge.svg)](https://github.com/yeganemehr/cpp-event-emitter/actions/workflows/build-examples.yml)

# General Information
I tried to build a similar API to [NodeJs's EventEmitter](https://nodejs.org/docs/latest/api/events.html#class-eventemitter) for handling events that can be very usefull in async projects.

Technically this code run on any platform but I did tested it only on esp8266 + Ardunio platform.   
This code is totally static and there is no dynamic memory allocation.

# Quickstart

Let's say we want to listening some data in Serial port and react to every new command.

```c++
#include <EventEmitter.hpp>
#include <Esp.h>

class SerialCommandEvent : public Event
{
public:
    static constexpr event_type_t type = 0x01;
    String command;
    SerialCommandEvent(const String &command) : command(command) {}
    event_type_t getType() const
    {
        return type;
    }
};

class MySerialCommander : public EventEmitter
{
private:
    String buffer;

public:
    void communicate()
    {
        while (Serial.available())
        {
            buffer.concat((char)Serial.read());
        }
        auto pos = buffer.indexOf('\n');
        while (pos != -1)
        {

            SerialCommandEvent event(buffer.substring(0, pos));
            event.command.trim();
            this->emit(&event);
            buffer.remove(0, pos + 1);

            pos = buffer.indexOf('\n');
        }
    }
};

void handleCommand(const SerialCommandEvent *e)
{
    if (e->command == "start")
    {
        Serial.println("Hello, it's a new begining");
    }
    else if (e->command == "quit")
    {
        Serial.println("Bye Bye!");
    }
    else
    {
        Serial.printf("Unknown command: %s\n", e->command.c_str());
    }
}

MySerialCommander myCommander;

void setup()
{
    Serial.begin(9600);
    myCommander.on(SerialCommandEvent::type, [](const Event *e) {
        handleCommand((const SerialCommandEvent *)e);
    });
}

void loop()
{
    myCommander.communicate();
}

```
You can build and upload this code to your board quickly with [Ready-To-Use Example](examples/esp8266-arduino)

## Methods & Helpers

**EventEmitter *EventEmitter::addListener(event_type_t type, listener_cb_t listener, bool once = false)**:  
Adds the listener function to the end of the listeners array for the event. No checks are made to see if the listener has already been added. Multiple calls passing the same combination of eventName and listener will result in the listener being added, and called, multiple times.

**EventEmitter *EventEmitter::prependListener(event_type_t type, listener_cb_t listener, bool once = false)**:  
Adds the listener function to the beginning of the listeners array for the event. No checks are made to see if the listener has already been added. Multiple calls passing the same combination of eventName and listener will result in the listener being added, and called, multiple times.

**EventEmitter *EventEmitter::prependOnceListener(event_type_t type, listener_cb_t listener)**:  
Adds a one-time listener function for the event to the beginning of the listeners array. The next time eventName is triggered, this listener is removed, and then invoked.

**EventEmitter *EventEmitter::removeAllListeners(event_type_t type)**: 
**EventEmitter *EventEmitter::removeAllListeners()**:  
Removes all listeners or those of the specified event.

It is bad practice to remove listeners added elsewhere in the code, particularly when the EventEmitter instance was created by some other component or module (e.g. sockets or file streams).

Returns a reference to the EventEmitter, so that calls can be chained.

**EventEmitter *EventEmitter::on(event_type_t type, listener_cb_t listener)**:   
Alias of `EventEmitter::addListener(type, listener, false)`.

**EventEmitter *EventEmitter::once(event_type_t type, listener_cb_t listener)**:   
Alias of `EventEmitter::addListener(type, listener, true)`.

**EventEmitter *EventEmitter::off(event_type_t type)**:   
Alias of `EventEmitter::removeAllListeners(type)`.

**EventEmitter *EventEmitter::emit(const Event *event)**:   
Synchronously calls each of the listeners registered for the event, in the order they were registered, passing the event pointer to each.

Returns true if the event had listeners, false otherwise.

# TODO:

* Unit tests.
* Add special `EventEmitter::ALL_EVENTS` event type to documention.

# License
The library is licensed under [MIT](LICENSE)