#include "menu.hpp"
#include "menu_pages.hpp"
#include "oled_sh1106.h"
#include "oled_sh1106_font.h"
#include "system_data.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace pedometer {

  enum : uint8_t { DEFAULT_LINK_PARAM_SELECT = 0 };

  uint8_t getCenteredTextCol(std::string text, uint8_t fontID) {
    switch(fontID) {
    case OLED_BASIC_FONT_ID:
      return (oled_get_attributes().columns - text.length()) / 2 * OLED_BASIC_FONT_COLS;
    default:
      return 0; // Unsupported font
    }
  }

  void Menu::init(ext_spi_handle_t ext_spi) {
    if(mIsInitialized) {
      throw std::runtime_error("MenuSystem instance is already initialized.");
    } else {
      // #################### Pages initialization and instatiation ####################
      // ################################## MAIN_PAGE ##################################

      // Helper vectors
      std::vector<std::unique_ptr<GraphicObject>> mainPageGO;
      std::vector<std::unique_ptr<InteractiveLink>> mainPageIL;

      // Graphic objects
      mainPageGO.emplace_back(std::make_unique<TextLine>("PEDOMETER", 1, getCenteredTextCol("PEDOMETER", OLED_BASIC_FONT_ID), OLED_BASIC_FONT_ID));
      mainPageGO.emplace_back(std::make_unique<TextLine>("STEPS:", 5, 72, OLED_BASIC_FONT_ID));
      mainPageGO.emplace_back(std::make_unique<TextLine>("TARGET:", 6, 72, OLED_BASIC_FONT_ID));

      mainPageGO.emplace_back(std::make_unique<DataLine>(5, OLED_BASIC_FONT_START_COL_OFFSET, OLED_BASIC_FONT_ID, DATA_STEPS));
      mainPageGO.emplace_back(std::make_unique<DataLine>(6, OLED_BASIC_FONT_START_COL_OFFSET, OLED_BASIC_FONT_ID, DATA_STEPS_TARGET));

      mainPageGO.emplace_back(std::make_unique<Rectangle>(
          Point{2, 2}, Point{static_cast<uint8_t>(oled_get_attributes().last_col - 2), static_cast<uint8_t>(oled_get_attributes().last_row - 2)}));

      // Links
      mainPageIL.emplace_back(
          std::make_unique<InteractiveLink>(CONFIG_PAGE, TextLine("CONFIG", 1, OLED_BASIC_FONT_START_COL_OFFSET, OLED_BASIC_FONT_ID)));

      // Calling page constructor
      std::unique_ptr<Page> mainPage = std::make_unique<LinkPage>(std::move(mainPageGO), std::move(mainPageIL));

      mainPage->setSelection(DEFAULT_LINK_PARAM_SELECT);

      //   -------------------------------------------------------------------------------

      // ################################## CONFIG_PAGE ##################################

      // Helper vectors
      std::vector<std::unique_ptr<GraphicObject>> configPageGO;
      std::vector<std::unique_ptr<InteractiveLink>> configPageIL;

      // Graphic objects
      configPageGO.emplace_back(std::make_unique<TextLine>("CONFIG", 1, getCenteredTextCol("CONFIG", OLED_BASIC_FONT_ID), OLED_BASIC_FONT_ID));

      configPageGO.emplace_back(std::make_unique<Rectangle>(
          Point{2, 2}, Point{static_cast<uint8_t>(oled_get_attributes().last_col - 2), static_cast<uint8_t>(oled_get_attributes().last_row - 2)}));

      // Links
      configPageIL.emplace_back(
          std::make_unique<InteractiveLink>(MAIN_PAGE, TextLine("BACK", 3, OLED_BASIC_FONT_START_COL_OFFSET, OLED_BASIC_FONT_ID)));
      configPageIL.emplace_back(
          std::make_unique<InteractiveLink>(MAIN_PAGE, TextLine("BACK 2", 4, OLED_BASIC_FONT_START_COL_OFFSET, OLED_BASIC_FONT_ID)));

      // Calling page constructor
      std::unique_ptr<Page> configPage = std::make_unique<LinkPage>(std::move(configPageGO), std::move(configPageIL));

      configPage->setSelection(DEFAULT_LINK_PARAM_SELECT);

      // -------------------------------------------------------------------------------

      // ############################ Active page selection ############################
      mActivePage = MAIN_PAGE;
      mIsInitialized = true;

      // Draw pages
      mPages[mActivePage]->updateDataAndParams(SystemData::GetInstance());
      mPages[mActivePage]->draw(ext_spi);
    }
  }

  void Menu::action(ext_spi_handle_t ext_spi, MenuAction action) {
    switch(action) {
    case MENU_ACTION_ENTER:
      if(auto page = dynamic_cast<LinkPage *>(mPages[mActivePage].get())) {
        mActivePage = page->getChild();
      } else if(auto page = dynamic_cast<ParamPage *>(mPages[mActivePage].get())) {
        if(page->getSelected() < (page->getParamsNum() - 1)) {
          page->changeSelection(true);
        } else {
          mActivePage = page->getParent();
        }
      } else {
        throw std::runtime_error("Invalid page type.");
      }
      break;

    case MENU_ACTION_UP:
      if(auto page = dynamic_cast<LinkPage *>(mPages[mActivePage].get())) {
        page->changeSelection(true);
      } else if(auto page = dynamic_cast<ParamPage *>(mPages[mActivePage].get())) {
        SystemData::GetInstance().changeValue(page->getSelectedParam()->getDataField(), true);
      } else {
        throw std::runtime_error("Invalid page type.");
      }
      break;

    case MENU_ACTION_DOWN:
      if(auto page = dynamic_cast<LinkPage *>(mPages[mActivePage].get())) {
        page->changeSelection(false);
      } else if(auto page = dynamic_cast<ParamPage *>(mPages[mActivePage].get())) {
        SystemData::GetInstance().changeValue(page->getSelectedParam()->getDataField(), false);
      } else {
        throw std::runtime_error("Invalid page type.");
      }
      break;

    default:
      throw std::invalid_argument("Invalid action.");
      break;
    }
    mPages[mActivePage]->updateDataAndParams(SystemData::GetInstance());
    mPages[mActivePage]->draw(ext_spi);
  }

} // namespace pedometer