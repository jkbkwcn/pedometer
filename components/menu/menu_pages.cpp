#include "menu_pages.hpp"
#include "oled_sh1106.h"
#include "oled_sh1106_font.h"
#include "system_data.hpp"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>

namespace pedometer {

  // Class: Point
  Point::Point(uint8_t x, uint8_t y) : mX(x), mY(y) { mColor = SHAPE_COLOR_WHITE; }
  Point::Point(uint8_t x, uint8_t y, ShapeColor color) : mX(x), mY(y), mColor(color) {}
  uint8_t Point::getX(void) const { return mX; }
  uint8_t Point::getY(void) const { return mY; }
  ShapeColor Point::getColor(void) const { return mColor; }
  void Point::setX(uint8_t x) { mX = x; }
  void Point::setY(uint8_t y) { mY = y; }
  void Point::setColor(ShapeColor color) { mColor = color; }
  void Point::draw(void) const {
    oled_point_t p = {.x = mX, .y = mY, .color = mColor};
    oled_put_point(p);
  }

  // Class: Line
  Line::Line(Point p, uint8_t length) : mStartP(p), mLength(length), mDir(SHAPE_LINE_VERTICAL), mColor(SHAPE_COLOR_WHITE) {}
  Line::Line(Point p, uint8_t length, LineDirection dir, ShapeColor color) : mStartP(p), mLength(length), mDir(dir), mColor(color) {}
  Point Line::getStartPoint(void) const { return mStartP; }
  uint8_t Line::getLength(void) const { return mLength; }
  LineDirection Line::getDirection(void) const { return mDir; }
  ShapeColor Line::getColor(void) const { return mColor; }
  void Line::setStartPoint(Point p) { mStartP = p; }
  void Line::setLength(uint8_t length) { mLength = length; }
  void Line::setDirection(LineDirection dir) { mDir = dir; }
  void Line::setColor(ShapeColor color) { mColor = color; }
  void Line::draw(void) const {
    oled_line_t line = (oled_line_t){.orig_x = mStartP.getX(), .orig_y = mStartP.getY(), .len = mLength, .color = mColor};
    if(SHAPE_LINE_VERTICAL == mDir) {
      oled_put_vline(line);
    } else if(SHAPE_LINE_HORIZONTAL == mDir) {
      oled_put_hline(line);
    }
  }

  // Class: Rectangle
  Rectangle::Rectangle(Point startP, Point endP) : mStartP(startP), mEndP(endP), mColor(SHAPE_COLOR_WHITE), mFill(SHAPE_NOT_FILLED) {}
  Rectangle::Rectangle(Point startP, Point endP, ShapeColor color, ShapeFill fill) : mStartP(startP), mEndP(endP), mColor(color), mFill(fill) {}
  Point Rectangle::getStartPoint(void) const { return mStartP; }
  Point Rectangle::getEndPoint(void) const { return mEndP; }
  ShapeColor Rectangle::getColor(void) const { return mColor; }
  ShapeFill Rectangle::getFill(void) const { return mFill; }
  void Rectangle::setStartPoint(Point p) { mStartP = p; }
  void Rectangle::setEndPoint(Point p) { mEndP = p; }
  void Rectangle::setColor(ShapeColor color) { mColor = color; }
  void Rectangle::setFill(ShapeFill fill) { mFill = fill; }
  void Rectangle::draw(void) const {
    oled_rect_t rect = (oled_rect_t){
        .orig_x = mStartP.getX(), .orig_y = mStartP.getY(), .end_x = mEndP.getX(), .end_y = mEndP.getY(), .color = mColor, .fill = mFill};
    oled_put_rect(rect);
  }

  // Class TextLine
  TextLine::TextLine(std::string text, uint8_t page, uint8_t col, uint8_t fontID) : mText(text), mPage(page), mCol(col), mFontID(fontID) {}
  std::string TextLine::getText(void) const { return mText; }
  uint8_t TextLine::getPage(void) const { return mPage; }
  uint8_t TextLine::getCol(void) const { return mCol; }
  uint8_t TextLine::getFontID(void) const { return mFontID; }
  void TextLine::setText(std::string text) { mText = text; }
  void TextLine::setPage(uint8_t page) { mPage = page; }
  void TextLine::setCol(uint8_t col) { mCol = col; }
  void TextLine::setFontID(uint8_t fontID) { mFontID = fontID; }
  void TextLine::draw(void) const { oled_put_string(mText.c_str(), mPage, mCol, mFontID); }

  // DataLine
  DataLine::DataLine(uint8_t page, uint8_t col, uint8_t fontID, DataField dataField)
      : mPage(page), mCol(col), mFontID(fontID), mDataField(dataField) {
    std::variant<uint8_t, uint32_t, bool> value = SystemData::GetInstance().getData(dataField);
    if(std::holds_alternative<uint8_t>(value) || std::holds_alternative<uint32_t>(value) || std::holds_alternative<bool>(value)) {
      mValue = value;
    } else {
      throw std::invalid_argument("Invalid argument type.");
    }
  }
  std::variant<uint8_t, uint32_t, bool> DataLine::getData(void) const {
    if(std::holds_alternative<uint8_t>(mValue)) {
      return std::get<uint8_t>(mValue);
    } else if(std::holds_alternative<uint32_t>(mValue)) {
      return std::get<uint32_t>(mValue);
    } else if(std::holds_alternative<bool>(mValue)) {
      return std::get<bool>(mValue);
    } else {
      throw std::runtime_error("Unable to get uninitialized value.");
    }
  }
  uint8_t DataLine::getPage(void) const { return mPage; }
  uint8_t DataLine::getCol(void) const { return mCol; }
  uint8_t DataLine::getFontID(void) const { return mFontID; }
  void DataLine::setData(std::variant<uint8_t, uint32_t, bool> value) {
    if(std::holds_alternative<uint8_t>(mValue) && std::holds_alternative<uint8_t>(value)) {
      mValue = value;
    } else if(std::holds_alternative<uint32_t>(mValue) && std::holds_alternative<uint32_t>(value)) {
      mValue = value;
    } else if(std::holds_alternative<bool>(mValue) && std::holds_alternative<bool>(value)) {
      mValue = value;
    } else {
      throw std::invalid_argument("Invalid argument type. Type of argument have to match current value type.");
    }
  }
  void DataLine::setPage(uint8_t page) { mPage = page; }
  void DataLine::setCol(uint8_t col) { mCol = col; }
  void DataLine::setFontID(uint8_t fontID) { mFontID = fontID; }
  void DataLine::updateParam(const SystemData &systemData) { mValue = systemData.getData(mDataField); }
  void DataLine::draw(void) const {
    if(std::holds_alternative<uint8_t>(mValue)) {
      oled_put_string(std::to_string(std::get<uint8_t>(mValue)).c_str(), mPage, mCol, mFontID);
    } else if(std::holds_alternative<uint32_t>(mValue)) {
      oled_put_string(std::to_string(std::get<uint32_t>(mValue)).c_str(), mPage, mCol, mFontID);
    } else {
      if(true == std::get<bool>(mValue)) {
        oled_put_string("YES", mPage, mCol, mFontID);
      } else {
        oled_put_string("NO", mPage, mCol, mFontID);
      }
    }
  }

  // InteractiveParam
  InteractiveParam::InteractiveParam(TextLine textLine, DataField dataField) : mTextLine(textLine), mDataField(dataField) {
    std::variant<uint8_t, uint32_t, bool> value = SystemData::GetInstance().getData(dataField);
    if(std::holds_alternative<uint8_t>(value) || std::holds_alternative<uint32_t>(value) || std::holds_alternative<bool>(value)) {
      mValue = value;
    } else {
      throw std::invalid_argument("Invalid argument type.");
    }
  }
  std::variant<uint8_t, uint32_t, bool> InteractiveParam::getValue(void) const {
    if(std::holds_alternative<uint8_t>(mValue)) {
      return std::get<uint8_t>(mValue);
    } else if(std::holds_alternative<uint32_t>(mValue)) {
      return std::get<uint32_t>(mValue);
    } else if(std::holds_alternative<bool>(mValue)) {
      return std::get<bool>(mValue);
    } else {
      throw std::runtime_error("Unable to get uninitialized value.");
    }
  }
  TextLine InteractiveParam::getLine(void) const { return mTextLine; }
  void InteractiveParam::setValue(std::variant<uint8_t, uint32_t, bool> value) {
    if(std::holds_alternative<uint8_t>(mValue) && std::holds_alternative<uint8_t>(value)) {
      mValue = value;
    } else if(std::holds_alternative<uint32_t>(mValue) && std::holds_alternative<uint32_t>(value)) {
      mValue = value;
    } else if(std::holds_alternative<bool>(mValue) && std::holds_alternative<bool>(value)) {
      mValue = value;
    } else {
      throw std::invalid_argument("Invalid argument type. Type of argument have to match current value type.");
    }
  }
  void InteractiveParam::setText(TextLine textLine) { mTextLine = textLine; }
  void InteractiveParam::updateParam(const SystemData &systemData) { mValue = systemData.getData(mDataField); }
  DataField InteractiveParam::getDataField(void) { return mDataField; }
  void InteractiveParam::draw(void) const {
    std::string value;
    if(std::holds_alternative<uint8_t>(mValue)) {
      value = std::to_string(std::get<uint8_t>(mValue));
    } else if(std::holds_alternative<uint32_t>(mValue)) {
      value = std::to_string(std::get<uint32_t>(mValue));
    } else {
      if(true == std::get<bool>(mValue)) {
        value = "YES";
      } else {
        value = "NO";
      }
    }
    std::string textAndValue = mTextLine.getText() + " " + value;
    oled_put_string(textAndValue.c_str(), mTextLine.getPage(), mTextLine.getCol(), mTextLine.getFontID());
  }

  // InteractiveLink
  InteractiveLink::InteractiveLink(PageName childPage, TextLine textLine) : mChildPage(childPage), mTextLine(textLine) {}
  PageName InteractiveLink::getPage(void) const { return mChildPage; }
  TextLine InteractiveLink::getLine(void) const { return mTextLine; }
  void InteractiveLink::setPage(PageName page) { mChildPage = page; }
  void InteractiveLink::setText(TextLine textLine) { mTextLine = textLine; }
  void InteractiveLink::draw(void) const { mTextLine.draw(); }

  // LinkPage
  LinkPage::LinkPage(std::vector<std::unique_ptr<GraphicObject>> &&graphic, std::vector<std::unique_ptr<InteractiveLink>> &&links)
      : mGraphic(std::move(graphic)), mLinks(std::move(links)), mSelected(INITIAL_SELECTION) {}
  void LinkPage::addGraphic(std::unique_ptr<GraphicObject> graphic) { mGraphic.emplace_back(std::move(graphic)); }
  void LinkPage::addLink(std::unique_ptr<InteractiveLink> link) {
    mLinks.emplace_back(std::move(link));
    // mLinks.emplace_back(std::move(link));
  }
  PageName LinkPage::getChild(void) { return mLinks[mSelected]->getPage(); }
  void LinkPage::updateDataAndParams(const SystemData &systemData) {
    for(const auto &obj : mGraphic) {
      if(auto dataLine = dynamic_cast<DataLine *>(obj.get())) {
        dataLine->updateParam(systemData);
      }
    }
  }
  void LinkPage::setCursor(void) const {
    oled_put_char('>', mLinks[mSelected]->getLine().getPage(), OLED_BASIC_FONT_START_COL, mLinks[mSelected]->getLine().getFontID());
  }
  void LinkPage::setSelection(uint8_t sel) { mSelected = sel; }
  void LinkPage::changeSelection(bool increase) {
    if(increase) {
      mSelected = std::clamp(static_cast<uint8_t>(mSelected + 1), static_cast<uint8_t>(0), static_cast<uint8_t>(mLinks.size()));
    } else {
      mSelected = std::clamp(static_cast<uint8_t>(mSelected - 1), static_cast<uint8_t>(0), static_cast<uint8_t>(mLinks.size()));
    }
  }
  InteractiveLink *LinkPage::getSelectedLink(void) { return mLinks[mSelected].get(); }
  void LinkPage::draw(ext_spi_handle_t ext_spi) const {
    for(const auto &obj : mGraphic) {
      obj->draw();
    }
    if(1 < mLinks.size()) {
      for(const auto &link : mLinks) {
        link->draw();
      }
      setCursor();
    }
    oled_render(ext_spi);
  }

  // ParamPage
  ParamPage::ParamPage(std::vector<std::unique_ptr<GraphicObject>> &&graphic, std::vector<std::unique_ptr<InteractiveParam>> &&params)
      : mGraphic(std::move(graphic)), mParams(std::move(params)), mSelected(INITIAL_SELECTION) {}
  void ParamPage::addGraphic(std::unique_ptr<GraphicObject> graphic) { mGraphic.emplace_back(std::move(graphic)); }
  void ParamPage::addParam(std::unique_ptr<InteractiveParam> param) { mParams.emplace_back(std::move(param)); }
  void ParamPage::updateDataAndParams(const SystemData &systemData) {
    for(const auto &obj : mGraphic) {
      if(auto dataLine = dynamic_cast<DataLine *>(obj.get())) {
        dataLine->updateParam(systemData);
      }
    }
    for(auto &param : mParams) {
      param->updateParam(systemData);
    }
  }
  void ParamPage::setCursor(void) const {
    oled_put_char('>', mParams[mSelected]->getLine().getPage(), OLED_BASIC_FONT_START_COL, mParams[mSelected]->getLine().getFontID());
  }
  PageName ParamPage::getParent(void) { return mParentPage; }
  uint8_t ParamPage::getSelected(void) { return mSelected; }
  uint8_t ParamPage::getParamsNum(void) { return mParams.size(); }
  void ParamPage::setSelection(uint8_t sel) { mSelected = sel; }
  void ParamPage::changeSelection(bool increase) {
    if(increase) {
      mSelected = std::clamp(static_cast<uint8_t>(mSelected + 1), static_cast<uint8_t>(0), static_cast<uint8_t>(mParams.size()));
    } else {
      mSelected = std::clamp(static_cast<uint8_t>(mSelected - 1), static_cast<uint8_t>(0), static_cast<uint8_t>(mParams.size()));
    }
  }
  InteractiveParam *ParamPage::getSelectedParam(void) { return mParams[mSelected].get(); }
  void ParamPage::draw(ext_spi_handle_t ext_spi) const {
    for(const auto &obj : mGraphic) {
      obj->draw();
    }
    for(const auto &param : mParams) {
      param->draw();
    }
    setCursor();
    oled_render(ext_spi);
  }

} // namespace pedometer
