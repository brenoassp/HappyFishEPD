#ifndef _ESP32HELPER_H_
#define _ESP32HELPER_H_

#define   _DC_PIN  5
#define  _SCK_PIN  7
#define _MISO_PIN  9
#define _MOSI_PIN 11
#define   _CS_PIN 12
#define   _SS_PIN 12
#define  _RST_PIN 16
#define  _SDA_PIN 33
#define  _SCL_PIN 35
#define _BUSY_PIN 39

#define  _D40_PIN 40
#define  _D38_PIN 38
#define  _D36_PIN 36
#define  _D34_PIN 34
#define  _D21_PIN 21
#define  _D17_PIN 17

#define SHT_0x44 SHTSensor::SHT3X
#define SHT_0x45 SHTSensor::SHT3X_ALT

#include <Arduino.h>
#include <elapsedMillis.h>

extern const unsigned long timeoutLength;

class Relay {
  byte _pin;
  bool _isOn = false;
  void write();

  public:
    Relay(const byte pin): _pin(pin) {};
    void setup();
    bool isOn();
    bool isOff();
    void turnOn();
    void turnOff();
    void toggle();
};

enum TstatMode {
  NONE,
  COOL,
  HEAT
};

enum TstatState {
  IDLE,
  WAIT,
  COOLING,
  HEATING
};

class Tstat {
  Relay _r;
  TstatMode _mode = TstatMode::NONE;
  TstatState _state = TstatState::IDLE;
  elapsedMillis _timeElapsed;
  float _setpoint;
  float _hysteresis;
  float _lowerLimit;
  float _upperLimit;
  unsigned long _interval;
  bool tooCold(const float cTemp);
  bool tooHot(const float cTemp);

  public:
    Tstat(const Relay &r): _r(r) {};
    void setup(const TstatMode mode,
               const float setpoint,
               const float hysteresis,
               const float lowerLimit,
               const float upperLimit,
               const unsigned long interval = timeoutLength);
    TstatState getState() const;
    void handle(const float cTemp);
};

#endif
