#include "clock_counter.hpp"
#include "clock_counter_config.hpp"
#include "system_data.hpp"
#include <variant>

using namespace pedometer;

void ClockCounter::init(void) {
  mCount = 0;
  mPresc = CLOCK_COUNTER_TICK_PRESCALER;
}

void ClockCounter::processTick() {
  mCount++;
  if(mCount >= mPresc) {
    mCount = 0;
    uint32_t seconds = std::get<uint32_t>(SystemData::GetInstance()->getData(DATA_SECONDS));
    uint32_t minutes = std::get<uint32_t>(SystemData::GetInstance()->getData(DATA_MINUTES));
    uint32_t hours = std::get<uint32_t>(SystemData::GetInstance()->getData(DATA_HOURS));
    if(SYSTEM_SECONDS_MAX == seconds) {
      SystemData::GetInstance()->setData(static_cast<uint32_t>(0), DATA_SECONDS);
      if(SYSTEM_MINUTES_MAX == minutes) {
        SystemData::GetInstance()->setData(static_cast<uint32_t>(0), DATA_MINUTES);
        if(SYSTEM_HOURS_MAX == hours) {
          SystemData::GetInstance()->setData(static_cast<uint32_t>(0), DATA_HOURS);
        } else {
          SystemData::GetInstance()->changeValue(DATA_HOURS, true);
        }
      } else {
        SystemData::GetInstance()->changeValue(DATA_MINUTES, true);
      }
    } else {
      SystemData::GetInstance()->changeValue(DATA_SECONDS, true);
    }
  }
}
