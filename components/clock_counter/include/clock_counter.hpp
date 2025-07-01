#ifndef CLOCK_COUNTER_H
#define CLOCK_COUNTER_H

#include "system_data.hpp"
#include <cstdint>
#include <string>

namespace pedometer {

  /**
   * @brief Class that represents system time counter: seconds, minutes and hours
   */
  class ClockCounter {
  private:
    volatile uint32_t mCount;
    uint32_t mPresc;

    // One default constructor, disable copying
    ClockCounter(void) = default;
    ClockCounter(const ClockCounter &) = delete;
    ClockCounter &operator=(const ClockCounter &) = delete;
    ClockCounter(ClockCounter &&) = delete;
    ClockCounter &operator=(ClockCounter &&) = delete;

  public:
    /**
     * @brief Static constructor for Singleton instance. After calling the constructor, method init() should be called once.
     */
    static ClockCounter &getInstance() {
      static ClockCounter *clockCounter = new ClockCounter();
      return *clockCounter;
    }

    /**
     * @brief Initializes clock counter with default values. Should be called only once after the constructor.
     * @note Calling the method more than once will throw std::runtime_error.
     */
    void init(void);

    /**
     * @brief Processes tick and changes system time.
     */
    void processTick(void);

    /**
     * @brief Should be called on interrupt from system counter.
     */
    void onTickISR(void);
  };

} // namespace pedometer

#endif // CLOCK_COUNTER_H
