#ifndef SYSTEM_DATA_H
#define SYSTEM_DATA_H

#include <cstdint>
#include <map>
#include <variant>

namespace pedometer {

  enum : bool { SYSTEM_DECREASE_VAL = false, SYSTEM_INCREASE_VAL = true };

  enum DataField : uint8_t { DATA_STEPS, DATA_TARGET_STEPS, DATA_HOURS, DATA_MINUTES, DATA_SECONDS };

  /**
   * @brief This is a template class for parameter that has its minimum and maximum value
   */
  class SystemParam {
  private:
    std::variant<uint8_t, uint32_t, bool> mValue;
    std::variant<uint8_t, uint32_t, bool> mMin;
    std::variant<uint8_t, uint32_t, bool> mMax;

  public:
    /**
     * @brief Class constructor
     * @note Value is set always within the limits
     */
    SystemParam(std::variant<uint8_t, uint32_t, bool> value, std::variant<uint8_t, uint32_t, bool> min, std::variant<uint8_t, uint32_t, bool> max);

    /**
     * @note Delete the default constructor, parameter instance has to be created with its value and boundaries
     */
    SystemParam() = delete;

    /**
     * @brief Returns the value
     * @return value
     */
    std::variant<uint8_t, uint32_t, bool> getValue(void) const;

    /**
     * @brief Sets the range - minimum and maximum values
     * @note Value is set always within the limits
     */
    void setValue(std::variant<uint8_t, uint32_t, bool> value);

    /**
     * @brief Sets the range - minimum and maximum values
     * @note Value is set always within the limits
     */
    void increaseValue(void);

    /**
     * @brief Sets the range - minimum and maximum values
     * @note Value is set always within the limits
     */
    void decreaseValue(void);
  };

  /**
   * @brief Class that represents system database.
   */
  class SystemData {
  private:
    std::map<DataField, SystemParam> mData;
    bool mIsInitialized = false;

    // One default constructor, disable copying
    SystemData(void) = default;
    SystemData(const SystemData &) = delete;
    SystemData &operator=(const SystemData &) = delete;
    SystemData(SystemData &&) = delete;
    SystemData &operator=(SystemData &&) = delete;

  public:
    /**
     * @brief Static constructor for Singleton instance. After calling the constructor, method init() should be called once.
     */
    static SystemData &GetInstance(void) {
      static SystemData *systemData = new SystemData();
      return *systemData;
    }

    /**
     * @brief Initializes system data with default values. Should be called only once after the constructor.
     * @note Calling the method more than once will throw std::runtime_error.
     */
    void init(void);

    /**
     * @brief Returns param value
     * @param dataField data field to return
     */
    std::variant<uint8_t, uint32_t, bool> getData(DataField dataField) const;

    /**
     * @brief Sets the data value in the given data field
     */
    void setData(std::variant<uint8_t, uint32_t, bool> value, DataField dataField);

    /**
     * @brief Changes the given value: increases or decreases it
     */
    void changeValue(DataField dataField, bool increase);
  };
} // namespace pedometer

#endif // SYSTEM_DATA_H
