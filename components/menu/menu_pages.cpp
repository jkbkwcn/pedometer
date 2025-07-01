#include "system_data.hpp"
#include "system_data_config.hpp"
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <type_traits>
#include <variant>

using namespace pedometer;

SystemParam::SystemParam(std::variant<uint8_t, uint32_t, bool> value, std::variant<uint8_t, uint32_t, bool> min,
                         std::variant<uint8_t, uint32_t, bool> max) {
  if(std::holds_alternative<uint8_t>(value) && std::holds_alternative<uint8_t>(min) && std::holds_alternative<uint8_t>(max)) {
    mMin = min;
    mMax = max;
    mValue = std::clamp(value, mMin, mMax);
  } else if(std::holds_alternative<uint32_t>(value) && std::holds_alternative<uint32_t>(min) && std::holds_alternative<uint32_t>(max)) {
    mMin = min;
    mMax = max;
    mValue = std::clamp(value, mMin, mMax);
  } else if(std::holds_alternative<bool>(value) && std::holds_alternative<bool>(min) && std::holds_alternative<bool>(max)) {
    mMin = false;
    mMax = true;
    mValue = value;
  } else {
    throw std::invalid_argument("All the arguments must be the same type. Available types: uint8_t, uint32_t or bool.");
  }
}

std::variant<uint8_t, uint32_t, bool> SystemParam::getValue(void) const {
  if(std::holds_alternative<uint8_t>(mValue) || std::holds_alternative<uint32_t>(mValue) || std::holds_alternative<bool>(mValue)) {
    return mValue;
  } else {
    return (uint8_t)0;
  }
}

void SystemParam::setValue(std::variant<uint8_t, uint32_t, bool> value) {
  if(std::holds_alternative<uint8_t>(mValue) && std::holds_alternative<uint8_t>(value)) {
    mValue = std::clamp(value, mMin, mMax);
  } else if(std::holds_alternative<uint32_t>(mValue) && std::holds_alternative<uint32_t>(value)) {
    mValue = std::clamp(value, mMin, mMax);
  } else if(std::holds_alternative<bool>(mValue) && std::holds_alternative<bool>(value)) {
    mValue = value;
  } else {
    throw std::invalid_argument("Argument must be of uint8_t, uint32_t or bool type and the same as currently stored type.");
  }
}

void SystemParam::increaseValue(void) {
  if(std::holds_alternative<uint8_t>(mValue)) {
    mValue = std::clamp(static_cast<uint8_t>(std::get<uint8_t>(mValue) + 1), std::get<uint8_t>(mMin), std::get<uint8_t>(mMax));
  } else if(std::holds_alternative<uint32_t>(mValue)) {
    mValue = std::clamp(static_cast<uint32_t>(std::get<uint32_t>(mValue) + 1), std::get<uint32_t>(mMin), std::get<uint32_t>(mMax));
  } else if(std::holds_alternative<bool>(mValue)) {
    if(false == std::get<bool>(mValue)) {
      mValue = true;
    }
  }
}

void SystemParam::decreaseValue(void) {
  if(std::holds_alternative<uint8_t>(mValue)) {
    mValue = std::clamp(static_cast<uint8_t>(std::get<uint8_t>(mValue) - 1), std::get<uint8_t>(mMin), std::get<uint8_t>(mMax));
  } else if(std::holds_alternative<uint32_t>(mValue)) {
    mValue = std::clamp(static_cast<uint32_t>(std::get<uint32_t>(mValue) - 1), std::get<uint32_t>(mMin), std::get<uint32_t>(mMax));
  } else if(std::holds_alternative<bool>(mValue)) {
    if(true == std::get<bool>(mValue)) {
      mValue = false;
    }
  }
}

void SystemData::init(void) {
  if(!mIsInitialized) {
    mData.insert({DATA_STEPS, SystemParam(static_cast<uint32_t>(SYSTEM_STEPS_MIN), static_cast<uint32_t>(SYSTEM_STEPS_DEFAULT),
                                          static_cast<uint32_t>(SYSTEM_STEPS_MAX))});
    mData.insert({DATA_STEPS_TARGET, SystemParam(static_cast<uint32_t>(SYSTEM_TARGET_STEPS_MIN), static_cast<uint32_t>(SYSTEM_TARGET_STEPS_DEFAULT),
                                                 static_cast<uint32_t>(SYSTEM_TARGET_STEPS_MAX))});
    mData.insert({DATA_HOURS, SystemParam(static_cast<uint8_t>(SYSTEM_HOURS_MIN), static_cast<uint8_t>(SYSTEM_HOURS_DEFAULT),
                                          static_cast<uint8_t>(SYSTEM_HOURS_MAX))});
    mData.insert({DATA_MINUTES, SystemParam(static_cast<uint8_t>(SYSTEM_MINUTES_MIN), static_cast<uint8_t>(SYSTEM_MINUTES_DEFAULT),
                                            static_cast<uint8_t>(SYSTEM_MINUTES_MAX))});
    mData.insert({DATA_SECONDS, SystemParam(static_cast<uint8_t>(SYSTEM_SECONDS_MIN), static_cast<uint8_t>(SYSTEM_SECONDS_DEFAULT),
                                            static_cast<uint8_t>(SYSTEM_SECONDS_MAX))});
    mIsInitialized = true;
  } else {
    throw std::runtime_error("SystemData instance is already initialized.");
  }
}

std::variant<uint8_t, uint32_t, bool> SystemData::getData(DataField dataField) const {
  auto data = mData.find(dataField);
  if(data != mData.end()) {
    if(std::holds_alternative<uint8_t>(data->second.getValue())) {
      return std::get<uint8_t>(data->second.getValue());
    } else if(std::holds_alternative<uint32_t>(data->second.getValue())) {
      return std::get<uint32_t>(data->second.getValue());
    } else if(std::holds_alternative<bool>(data->second.getValue())) {
      return std::get<bool>(data->second.getValue());
    } else {
      // throw std::invalid_argument("Invalid key.");
      return (uint8_t)0;
    }
  } else {
    throw std::invalid_argument("Invalid key.");
  }
}

void SystemData::setData(std::variant<uint8_t, uint32_t, bool> value, DataField dataField) {
  auto data = mData.find(dataField);
  if(data != mData.end()) {
    if(std::holds_alternative<uint8_t>(data->second.getValue())) {
      data->second.setValue(std::get<uint8_t>(value));
    } else if(std::holds_alternative<uint32_t>(data->second.getValue())) {
      data->second.setValue(std::get<uint32_t>(value));
    } else if(std::holds_alternative<bool>(data->second.getValue())) {
      data->second.setValue(std::get<bool>(value));
    }
  } else {
    throw std::invalid_argument("Invalid key.");
  }
}

void SystemData::changeValue(DataField dataField, bool increase) {
  auto data = mData.find(dataField);
  if(data != mData.end()) {
    if(SYSTEM_INCREASE_VAL == increase) {
      data->second.increaseValue();
    } else {
      data->second.decreaseValue();
    }
  } else {
    throw std::invalid_argument("Invalid key.");
  }
}
