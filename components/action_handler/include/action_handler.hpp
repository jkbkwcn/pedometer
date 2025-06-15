#ifndef ACTION_HANDLER_H
#define ACTION_HANDLER_H

#include "driver/gpio.h"
#include "menu.hpp"
#include <optional>
#include <vector>

namespace pedometer {
  class InputPin {
  private:
    gpio_num_t mPin;
    bool mHighTrue;
    bool mState;
    bool mPrevState;
    bool mPrevPrevState;
    MenuAction mAction;

  public:
    /**
     * @brief Constructor.
     */
    InputPin(gpio_num_t pin, bool highTrue, MenuAction action);

    /**
     * @brief Gets the pin number.
     */
    gpio_num_t getPin(void) const;

    /**
     * @brief Gets the logic direction.
     */
    bool getHighTrue(void) const;

    /**
     * @brief Gets the previous, previous state.
     */
    bool getState(void) const;

    /**
     * @brief Gets the previous, previous state.
     */
    bool getPrevState(void) const;

    /**
     * @brief Gets the previous, previous state.
     */
    bool getPrevPrevState(void) const;

    /**
     * @brief Gets the MenuAction related to pin.
     */
    MenuAction getMenuAction(void) const;

    /**
     * @brief Sets the state.
     */
    void setState(bool state);

    /**
     * @brief Sets the previous state.
     */
    void setPrevState(bool state);

    /**
     * @brief Sets the previous, previous state.
     */
    void setPrevPrevState(bool state);
  }

  class ActionHandler {
  private:
    std::vector<InputPin> mPins;
    uint8_t mDebounceTime;

  public:
    /**
     * @brief Constructor.
     */
    ActionHandler(std::vector<InputPin> pins, uint8_t debounceTime);

    /**
     * @brief Evaluates action and returns MenuAction after scanning.
     */
    std::optional<MenuAction> evaluateAction(void);

    /**
     * @brief Scans pins and sets their state and previous state params.
     */
    void scan(void);
  }
} // namespace pedometer

#endif // ACTION_HANDLER_H
