#ifndef MENU_H
#define MENU_H

#include "menu_pages.hpp"
#include <map>
#include <memory>

namespace pedometer {

  enum MenuAction { MENU_ACTION_ENTER, MENU_ACTION_UP, MENU_ACTION_DOWN };

  /**
   * @brief Class that represents the whole menu system.
   */
  class Menu {
  private:
    std::map<PageName, std::unique_ptr<Page>> mPages;
    PageName mActivePage;
    bool mIsInitialized;

    // One default constructor, disable copying
    Menu(void) = default;
    Menu(const Menu &) = delete;
    Menu &operator=(const Menu &) = delete;
    Menu(Menu &&) = delete;
    Menu &operator=(Menu &&) = delete;

  public:
    /**
     * @brief Static constructor for Singleton instance. After calling the constructor, method init() should be called once.
     */
    static Menu &GetInstance(void) {
      static Menu *menuSystem = new Menu();
      return *menuSystem;
    }

    /**
     * @brief Initializes the menu system. Should be called only once after the constructor.
     * @note Calling the method more than once will throw std::runtime_error.
     */
    void init(ext_spi_handle_t ext_spi);

    /**
     * @brief Enables an action in a menu system
     * @param action action: ENTER, UP, DOWN (...)
     */
    void action(ext_spi_handle_t ext_spi, MenuAction action);
  };
} // namespace pedometer

#endif // MENU_H
