#include "ESP32Helper.h"

const unsigned long timeoutLength = 60000;
ESP32Time rtc;

void Relay::setup() {
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  write();
}

bool Relay::isOn() {
  return _isOn;
}

bool Relay::isOff() {
  return !_isOn;
}

void Relay::turnOn() {
  if (!_isOn) {
    _isOn = true;
    write();
  }
}

void Relay::turnOff() {
  if (_isOn) {
    _isOn = false;
    write();
  }
}

void Relay::toggle() {
  _isOn ? turnOff() : turnOn();
}

void Relay::write() {
  digitalWrite(_pin, _isOn ? LOW : HIGH);
}

void Tstat::setup(const TstatMode mode,
                  const float setpoint,
                  const float hysteresis,
                  const float lowerLimit,
                  const float upperLimit,
                  const unsigned long interval
) {
  _state = TstatState::IDLE;
  _mode = mode;
  _setpoint = setpoint;
  _hysteresis = hysteresis;
  _lowerLimit = lowerLimit;
  _upperLimit = upperLimit;
  _interval = interval < timeoutLength ? timeoutLength : interval;
}

TstatState Tstat::getState() const {
  return _state;
}

void Tstat::handle(const float cTemp) {
  if (cTemp < _lowerLimit || cTemp > _upperLimit) {
    _state = TstatState::IDLE;
    _r.turnOff();
  }
  if (_mode != TstatMode::NONE) {
    switch(_state) {
      case TstatState::COOLING:
        if (cTemp <= _setpoint-_hysteresis) {
          _state = TstatState::IDLE;
          _r.turnOff();
        }
        break;
      case TstatState::HEATING:
        if (cTemp >= _setpoint+_hysteresis) {
          _state = TstatState::IDLE;
          _r.turnOff();
        }
        break;
      case TstatState::WAIT:
        if (_timeElapsed > _interval) {
          TstatState newState = _state;
          if (tooHot(cTemp)) newState = TstatState::COOLING;
          else if (tooCold(cTemp)) newState = TstatState::HEATING;
          if (newState == _state) {
            _state = TstatState::IDLE;
          } else {
            _r.turnOn();
            _state = newState;
          }
        }
        break;
      default:
        if (tooCold(cTemp) || tooHot(cTemp)) {
          _timeElapsed = 0;
          _state = TstatState::WAIT;
        }
    }
  }
}

bool Tstat::tooCold(const float cTemp) {
  return _mode == TstatMode::HEAT && cTemp <= _setpoint;
}

bool Tstat::tooHot(const float cTemp) {
  return _mode == TstatMode::COOL && cTemp >= _setpoint;
}