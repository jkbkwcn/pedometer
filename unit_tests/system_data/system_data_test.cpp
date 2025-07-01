#include "system_data.hpp"
#include "system_data_config.hpp"
#include <gtest/gtest.h>

using namespace pedometer;

// -------------------------------------------------------------------------------
// ------------------------- SystemParam class unit test -------------------------
// -------------------------------------------------------------------------------
TEST(SystemParamTest, ParamUint8Test) {
  // Parameter type <uint8_t> test
  SystemParam param(static_cast<uint8_t>(10), static_cast<uint8_t>(5), static_cast<uint8_t>(15));
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 10);

  // Maximum value clamp test
  param.setValue(static_cast<uint8_t>(20));
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 15);

  // Minimum value clamp test
  param.setValue(static_cast<uint8_t>(0));
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 5);

  // Maximum value clamp test using increase method
  param.setValue(static_cast<uint8_t>(14));
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 14);
  param.increaseValue();
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 15);
  param.increaseValue();
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 15);

  // Minimum value clamp test using decrease method
  param.setValue(static_cast<uint8_t>(6));
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 6);
  param.decreaseValue();
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 5);
  param.decreaseValue();
  EXPECT_EQ(std::get<uint8_t>(param.getValue()), 5);
}

TEST(SystemParamTest, ParamUint32Test) {
  // Parameter type <uint32_t> test
  SystemParam param(static_cast<uint32_t>(10), static_cast<uint32_t>(5), static_cast<uint32_t>(15));
  EXPECT_EQ(std::get<uint32_t>(param.getValue()), 10);

  // Maximum value clamp test
  param.setValue(static_cast<uint32_t>(20));
  EXPECT_EQ(std::get<uint32_t>(param.getValue()), 15);

  // Minimum value clamp test
  param.setValue(static_cast<uint32_t>(0));
  EXPECT_EQ(std::get<uint32_t>(param.getValue()), 5);

  // Maximum value clamp test using increase method
  param.setValue(static_cast<uint32_t>(14));
  EXPECT_EQ(std::get<uint32_t>(param.getValue()), 14);
  param.increaseValue();
  EXPECT_EQ(std::get<uint32_t>(param.getValue()), 15);
  param.increaseValue();
  EXPECT_EQ(std::get<uint32_t>(param.getValue()), 15);

  // Minimum value clamp test using decrease method
  param.setValue(static_cast<uint32_t>(6));
  EXPECT_EQ(std::get<uint32_t>(param.getValue()), 6);
  param.decreaseValue();
  EXPECT_EQ(std::get<uint32_t>(param.getValue()), 5);
  param.decreaseValue();
  EXPECT_EQ(std::get<uint32_t>(param.getValue()), 5);
}

TEST(SystemParamTest, ParamBooleanTest) {
  // Parameter type <bool> test
  // Constructore should ignore minimum and maximum values
  SystemParam param(static_cast<bool>(false), static_cast<bool>(false), static_cast<bool>(false));
  EXPECT_EQ(std::get<bool>(param.getValue()), false);

  // Seting value test
  param.setValue(static_cast<bool>(true));
  EXPECT_EQ(std::get<bool>(param.getValue()), true);
  param.setValue(static_cast<bool>(false));
  EXPECT_EQ(std::get<bool>(param.getValue()), false);

  // Maximum value clamp test using increase method
  param.setValue(static_cast<bool>(false));
  EXPECT_EQ(std::get<bool>(param.getValue()), false);
  param.increaseValue();
  EXPECT_EQ(std::get<bool>(param.getValue()), true);
  param.increaseValue();
  EXPECT_EQ(std::get<bool>(param.getValue()), true);

  // Minimum value clamp test using decrease method
  param.setValue(static_cast<bool>(true));
  EXPECT_EQ(std::get<bool>(param.getValue()), true);
  param.decreaseValue();
  EXPECT_EQ(std::get<bool>(param.getValue()), false);
  param.decreaseValue();
  EXPECT_EQ(std::get<bool>(param.getValue()), false);
}

TEST(SystemParamTest, ParamMixedTypesTest) {
  EXPECT_THROW(SystemParam(static_cast<uint8_t>(5), static_cast<uint32_t>(0), static_cast<uint32_t>(10)), std::invalid_argument);

  EXPECT_THROW(SystemParam(static_cast<bool>(5), static_cast<bool>(0), static_cast<uint32_t>(10)), std::invalid_argument);
}

// -------------------------------------------------------------------------------
// -------------------------- SystemData class unit test -------------------------
// -------------------------------------------------------------------------------
TEST(SystemDataTest, InstantiationTest) {
  SystemData &instance1 = SystemData::GetInstance();
  SystemData &instance2 = SystemData::GetInstance();

  // Check if this is the same object (singleton)
  EXPECT_EQ(&instance1, &instance2);
}

TEST(SystemDataTest, InitializationTest) {
  SystemData &data = SystemData::GetInstance();
  data.init();

  // Checking default initailization data
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_STEPS)), SYSTEM_STEPS_DEFAULT);
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_TARGET_STEPS)), SYSTEM_TARGET_STEPS_DEFAULT);
  EXPECT_EQ(std::get<uint8_t>(data.getData(DATA_HOURS)), SYSTEM_HOURS_DEFAULT);
  EXPECT_EQ(std::get<uint8_t>(data.getData(DATA_MINUTES)), SYSTEM_MINUTES_DEFAULT);
  EXPECT_EQ(std::get<uint8_t>(data.getData(DATA_SECONDS)), SYSTEM_SECONDS_DEFAULT);

  // Initialization should be performed only once
  EXPECT_THROW(data.init(), std::runtime_error);
}

TEST(SystemDataTest, SetAndGetDataTest) {
  SystemData &data = SystemData::GetInstance();
  data.init();

  // Setting param tests
  data.setData(static_cast<uint32_t>(1234), DATA_STEPS);
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_STEPS)), 1234);

  data.setData(static_cast<uint32_t>(1000), DATA_TARGET_STEPS);
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_TARGET_STEPS)), 1000);

  data.setData(static_cast<uint8_t>(12), DATA_HOURS);
  EXPECT_EQ(std::get<uint8_t>(data.getData(DATA_HOURS)), 12);

  data.setData(static_cast<uint8_t>(10), DATA_MINUTES);
  EXPECT_EQ(std::get<uint8_t>(data.getData(DATA_MINUTES)), 10);

  data.setData(static_cast<uint8_t>(50), DATA_SECONDS);
  EXPECT_EQ(std::get<uint8_t>(data.getData(DATA_SECONDS)), 50);
}

TEST(SystemDataTest, ChangeValueAndClampingTest) {
  SystemData &data = SystemData::GetInstance();
  data.init();

  // Decreasing value with clamping
  data.setData(static_cast<uint32_t>(SYSTEM_STEPS_MIN + 1), DATA_STEPS);
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_STEPS)), SYSTEM_STEPS_MIN + 1);
  data.changeValue(DATA_STEPS, SYSTEM_DECREASE_VAL);
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_STEPS)), SYSTEM_STEPS_MIN);
  data.changeValue(DATA_STEPS, SYSTEM_DECREASE_VAL);
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_STEPS)), SYSTEM_STEPS_MIN);

  // Increasing value with clamping
  data.setData(static_cast<uint32_t>(SYSTEM_STEPS_MAX - 1), DATA_STEPS);
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_STEPS)), SYSTEM_STEPS_MAX - 1);
  data.changeValue(DATA_STEPS, SYSTEM_INCREASE_VAL);
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_STEPS)), SYSTEM_STEPS_MAX);
  data.changeValue(DATA_STEPS, SYSTEM_INCREASE_VAL);
  EXPECT_EQ(std::get<uint32_t>(data.getData(DATA_STEPS)), SYSTEM_STEPS_MAX);
}
