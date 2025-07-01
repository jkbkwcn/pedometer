#include "system_data.hpp"
#include "system_data_config.hpp"
#include <gtest/gtest.h>

using namespace pedometer;

TEST(SystemParamTest, Uint8ClampTest) {
  SystemParam param(static_cast<uint8_t>(10), static_cast<uint8_t>(0), static_cast<uint8_t>(20));
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 10);

  param.setValue(static_cast<uint8_t>(25)); // większe niż max
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 20);

  param.setValue(static_cast<uint8_t>(-5)); // mniejsze niż min (zawinięte jako uint8)
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 0);
}

TEST(SystemParamTest, IncreaseDecreaseUint8) {
  SystemParam param(static_cast<uint8_t>(10), static_cast<uint8_t>(0), static_cast<uint8_t>(20));
  param.increaseValue();
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 11);

  param.decreaseValue();
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 10);
}

TEST(SystemParamTest, BoolBehavior) {
  SystemParam param(false, false, true);
  EXPECT_EQ(std::get<bool>(param.getValue()), false);

  param.increaseValue();
  EXPECT_EQ(std::get<bool>(param.getValue()), true);

  param.decreaseValue();
  EXPECT_EQ(std::get<bool>(param.getValue()), false);
}

TEST(SystemParamTest, ThrowsOnMixedTypes) {
  EXPECT_THROW(SystemParam(static_cast<uint8_t>(5), static_cast<uint32_t>(0), static_cast<uint32_t>(10)), std::invalid_argument);
}

TEST(SystemDataTest, Initialization) {
  SystemData &data = SystemData::GetInstance();
  data.init();

  EXPECT_NO_THROW(data.getData(DATA_STEPS));
  EXPECT_THROW(data.init(), std::runtime_error); // już zainicjowane
}

TEST(SystemDataTest, SetAndGetData) {
  SystemData &data = SystemData::GetInstance();

  data.setData(static_cast<uint32_t>(1234), DATA_STEPS);
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_STEPS)), 1234);

  data.setData(static_cast<uint8_t>(12), DATA_HOURS);
  EXPECT_EQ(std::get<uint8_t>(data.getData(DATA_HOURS)), 12);
}

TEST(SystemDataTest, ChangeValue) {
  SystemData &data = SystemData::GetInstance();

  uint8_t before = std::get<uint8_t>(data.getData(DATA_MINUTES));
  data.changeValue(DATA_MINUTES, SYSTEM_INCREASE_VAL);
  EXPECT_EQ(std::get<uint8_t>(data.getData(DATA_MINUTES)), std::min<uint8_t>(before + 1, SYSTEM_MINUTES_MAX));
}
