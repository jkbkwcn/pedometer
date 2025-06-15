#include "action_handler.hpp"
#include "FreeRTOS/task.h"
#include "driver/gpio.h"
#include <optional>
#include <vector>

using namespace pedometer;

// InputPin
InputPin::InputPin(gpio_num_t pin, bool highTrue, MenuAction action)
    : mPin(pin), mHighTrue(highTrue), mState(!highTrue), mPrevState(!highTrue), mPrevPrevState(!highTrue), mAction(action) {}
gpio_num_t InputPin::getPin(void) const { return mPin; }
bool InputPin::getHighTrue(void) const { return mHighTrue; }
bool InputPin::getState(void) const { return mState; }
bool InputPin::getPrevState(void) const { return mPrevState; }
bool InputPin::getPrevPrevState(void) const { return mPrevPrevState; }
MenuAction InputPin::getMenuAction(void) const { return mAction; }
void InputPin::setState(bool state) { mState = state; }
void InputPin::setPrevState(bool state) { mPrevState = state; }
void InputPin::setPrevPrevState(bool state) { mPrevPrevState = state; }

// ActionHandler
ActionHandler::ActionHandler(std::vector<InputPin> pins, uint8_t debounceTime) : mPins(pins) {}
std::optional<MenuAction> ActionHandler::evaluateAction(void) {
  for(auto pin : mPins) {
    if(pin.getPrevPrevState() != = pin.getHighTrue() && pin.getPrevState() == pin.getHighTrue() && pin.getState() == pin.getHighTrue()) {
      return pin.getMenuAction();
    }
  }
  return std::nullopt;
}
void ActionHandler::scan(void) {
  bool tempState;
  for(auto pin : mPins) {
    pin.setPrevPrevState(pin.getPrevState());
    pin.setPrevState(pin.getState());
    pin.setState(gpio_get_level(pin.getPin()));
  }
}
