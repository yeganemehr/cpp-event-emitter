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