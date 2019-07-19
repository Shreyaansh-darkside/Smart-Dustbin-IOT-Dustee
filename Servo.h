#ifndef Servo_h
#define Servo_h

#include <Arduino.h>

// the following are in us (microseconds)
//
#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds 

#if !defined(ESP8266)

#error "This library only supports esp8266 boards."

#endif

class Servo
{
public:
    Servo();
    ~Servo();
    uint8_t attach(int pin);           // attach the given pin to the next free channel, sets pinMode, returns channel number or 0 if failure
    uint8_t attach(int pin, uint16_t min, uint16_t max); // as above but also sets min and max values for writes. 
    void detach();
    void write(int value);             // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 
    void writeMicroseconds(int value); // Write pulse width in microseconds 
    int read();                        // returns current pulse width as an angle between 0 and 180 degrees
    int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
    bool attached();                   // return true if this servo is attached, otherwise false 
private:
    bool     _attached;
    uint8_t  _pin;
    uint16_t _minUs;                   
    uint16_t _maxUs;                   
    uint16_t _valueUs;
};

#endif
