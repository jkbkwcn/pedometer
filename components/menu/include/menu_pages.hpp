#ifndef MENU_PAGES_H
#define MENU_PAGES_H

#include "oled_sh1106.h"
#include "oled_sh1106_config.h"
#include "system_data.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <variant>
#include <vector>

// enum { OLED_WIDTH = 128, OLED_HEIGHT = 64, OLED_NUM_PAGES = 8 }

namespace pedometer {

  enum LineDirection : bool { SHAPE_LINE_VERTICAL = true, SHAPE_LINE_HORIZONTAL = false };
  enum ShapeColor : bool { SHAPE_COLOR_WHITE = true, SHAPE_COLOR_BLACK = false };
  enum ShapeFill : bool { SHAPE_FILLED = true, SHAPE_NOT_FILLED = false };
  enum { INITIAL_SELECTION = 0 };

  enum VariantType { VARIANT_UINT8_T, VARIANT_UINT32_T, VARIANT_BOOL };

  // Pages are stored in the Menu class in the map<PageName, Page>, where PageName type variable is the key
  enum PageName { MAIN_PAGE, CONFIG_PAGE };

  // ----- Forward declaration -----
  class Page;
  // -------------------------------

  /**
   * @brief This class represents a GraphicObject - shape ot text to draw on the display.
   */
  class GraphicObject {
  public:
    // Virtual methods
    virtual void draw(void) const = 0;
    virtual ~GraphicObject() = default;
  };

  /**
   * @brief This class represents a single point to draw on the display.
   */
  class Point : public GraphicObject {
  private:
    uint8_t mX;
    uint8_t mY;
    ShapeColor mColor;

  public:
    /**
     * @brief Object constructor. By default point is white.
     */
    Point(uint8_t x, uint8_t y);

    /**
     * @brief Object constructor.
     */
    Point(uint8_t x, uint8_t y, ShapeColor color);

    /**
     * Default destructor.
     */
    ~Point() override = default;

    /**
     * @brief Returns x coordinate of the point.
     */
    uint8_t getX(void) const;

    /**
     * @brief Returns y coordinate of the point.
     */
    uint8_t getY(void) const;

    /**
     * @brief Returns the point color.
     * @return SHAPE_COLOR_WHITE if the point white; SHAPE_COLOR_BLACK if it is black
     */
    ShapeColor getColor(void) const;

    /**
     * @brief Sets the x coordinate of the point.
     * @return x coordinate
     */
    void setX(uint8_t x);

    /**
     * @brief Sets the y coordinate of the point.
     * @return y coordinate
     */
    void setY(uint8_t y);

    /**
     * @brief Sets the the point color: SHAPE_COLOR_WHITE is white; SHAPE_COLOR_BLACK is black
     */
    void setColor(ShapeColor color);

    /**
     * @brief Puts the point into the video buffer.
     */
    void draw(void) const override;
  };

  /**
   * @brief This class represents a vertital or a horizontal line to draw on the display.
   */
  class Line : public GraphicObject {
  private:
    Point mStartP;
    uint8_t mLength;
    LineDirection mDir;
    ShapeColor mColor;

  public:
    /**
     * @brief Object constructor. By default line is white and vertical.
     */
    Line(Point p, uint8_t length);

    /**
     * @brief Object constructor.
     */
    Line(Point p, uint8_t length, LineDirection dir, ShapeColor color);

    /**
     * Default destructor.
     */
    ~Line() override = default;

    /**
     * @brief Returns the start point.
     * @return start point
     */
    Point getStartPoint(void) const;

    /**
     * @brief Returns the length of the line.
     * @return length
     */
    uint8_t getLength(void) const;

    /**
     * @brief Returns if line is vertical or horizontal.
     * @return SHAPE_LINE_VERTICAL if the line is vertical; SHAPE_LINE_HORIZONTAL if it is horizontal
     */
    LineDirection getDirection(void) const;

    /**
     * @brief Returns the line color.
     * @return HAPE_COLOR_WHITE if the line white; SHAPE_COLOR_BLACK if it is black
     */
    ShapeColor getColor(void) const;

    /**
     * @brief Sets the start point of the line.
     */
    void setStartPoint(Point p);

    /**
     * @brief Sets the length of the line.
     */
    void setLength(uint8_t length);

    /**
     * @brief Sets the the direction of the line: SHAPE_LINE_VERTICAL is vertical; SHAPE_LINE_HORIZONTAL is horizontal.
     */
    void setDirection(LineDirection dir);

    /**
     * @brief Sets the the line color: SHAPE_COLOR_WHITE is white; SHAPE_COLOR_BLACK is black.
     */
    void setColor(ShapeColor color);

    /**
     * @brief Puts the line into the video buffer.
     */
    void draw(void) const override;
  };

  /**
   * @brief This class represents a filled or a not filled rectangle to draw on the display.
   */
  class Rectangle : public GraphicObject {
  private:
    Point mStartP;
    Point mEndP;
    ShapeColor mColor;
    ShapeFill mFill;

  public:
    /**
     * @brief Object constructor. By default color is white and rectangle is not filled.
     */
    Rectangle(Point startP, Point endP);

    /**
     * @brief Object constructor.
     */
    Rectangle(Point startP, Point endP, ShapeColor color, ShapeFill fill);

    /**
     * Default destructor.
     */
    ~Rectangle() override = default;

    /**
     * @brief Returns the start point.
     * @return start point
     */
    Point getStartPoint(void) const;

    /**
     * @brief Returns the end point.
     * @return end point
     */
    Point getEndPoint(void) const;

    /**
     * @brief Returns the rectangle color.
     * @return SHAPE_COLOR_WHITE if the rectangle white; SHAPE_COLOR_BLACK if it is black
     */
    ShapeColor getColor(void) const;

    /**
     * @brief Returns the rectangle fill.
     * @return SHAPE_FILLED if the rectangle is filled; SHAPE_NOT_FILLED if it is not filled
     */
    ShapeFill getFill(void) const;

    /**
     * @brief Sets the start point of the rectangle.
     */
    void setStartPoint(Point p);

    /**
     * @brief Sets the end point of the rectangle.
     */
    void setEndPoint(Point p);

    /**
     * @brief Sets the the rectangle fill: SHAPE_FILLED is filled; SHAPE_NOT_FILLED is not filled.
     */
    void setFill(ShapeFill fill);

    /**
     * @brief Sets the the rectangle color: SHAPE_COLOR_WHITE is white; SHAPE_COLOR_BLACK is black.
     */
    void setColor(ShapeColor color);

    /**
     * @brief Puts the rectangle into the video buffer.
     */
    void draw(void) const override;
  };

  /**
   * @brief This class represents a single string to print on the display.
   */
  class TextLine : public GraphicObject {
  private:
    std::string mText;
    uint8_t mPage;
    uint8_t mCol;
    uint8_t mFontID;

  public:
    /**
     * @brief Object constructor.
     */
    TextLine(std::string text, uint8_t page, uint8_t col, uint8_t fontID);

    /**
     * Default destructor.
     */
    ~TextLine() override = default;

    /**
     * @brief Returns the text string.
     */
    std::string getText(void) const;

    /**
     * @brief Returns the text page.
     */
    uint8_t getPage(void) const;

    /**
     * @brief Returns the text column.
     */
    uint8_t getCol(void) const;

    /**
     * @brief Returns the text font ID.
     */
    uint8_t getFontID(void) const;

    /**
     * @brief Sets the text.
     */
    void setText(std::string text);

    /**
     * @brief Sets the text page.
     */
    void setPage(uint8_t page);

    /**
     * @brief Sets the text column.
     */
    void setCol(uint8_t col);

    /**
     * @brief Sets the text font ID.
     */
    void setFontID(uint8_t fontID);

    /**
     * @brief Puts the text into the video buffer.
     */
    void draw(void) const override;
  };

  /**
   * @brief This class represents a single data line to print on the display.
   */
  class DataLine : public GraphicObject {
  private:
    std::variant<uint8_t, uint32_t, bool> mValue;
    uint8_t mPage;
    uint8_t mCol;
    uint8_t mFontID;
    DataField mDataField;

  public:
    /**
     * @brief Object constructor.
     */
    DataLine(uint8_t page, uint8_t col, uint8_t fontID, DataField dataField);

    /**
     * Default destructor.
     */
    ~DataLine() override = default;

    /**
     * @brief Returns the data value.
     */
    std::variant<uint8_t, uint32_t, bool> getData(void) const;

    /**
     * @brief Returns the data page.
     */
    uint8_t getPage(void) const;

    /**
     * @brief Returns the data column.
     */
    uint8_t getCol(void) const;

    /**
     * @brief Returns the data font ID.
     */
    uint8_t getFontID(void) const;

    /**
     * @brief Sets the data value.
     */
    void setData(std::variant<uint8_t, uint32_t, bool> value);

    /**
     * @brief Sets the data page.
     */
    void setPage(uint8_t page);

    /**
     * @brief Sets the data column.
     */
    void setCol(uint8_t col);

    /**
     * @brief Sets the data font ID.
     */
    void setFontID(uint8_t fontID);

    /**
     * @brief Updates parameter from system data base.
     * @param systemData pointer to the system data base
     */
    void updateParam(const SystemData &systemData);

    /**
     * @brief Puts the data converted to text into the video buffer.
     */
    void draw(void) const override;
  };

  /**
   * @brief This class represents a parameter or an option that can be changed.
   */
  class InteractiveParam {
  private:
    std::variant<uint8_t, uint32_t, bool> mValue;
    TextLine mTextLine;
    DataField mDataField;

  public:
    /**
     * @brief Object constructor.
     */
    InteractiveParam(TextLine textLine, DataField dataField);

    /**
     * Default destructor.
     */
    ~InteractiveParam() = default;

    /**
     * @brief Returns the param value.
     */
    std::variant<uint8_t, uint32_t, bool> getValue(void) const;

    /**
     * @brief Returns text of the param.
     */
    TextLine getLine(void) const;

    /**
     * @brief Sets the param value.
     */
    void setValue(std::variant<uint8_t, uint32_t, bool> value);

    /**
     * @brief Sets the text of the param.
     */
    void setText(TextLine textLine);

    /**
     * @brief Updates parameter from system data base.
     * @param systemData pointer to the system data base
     */
    void updateParam(const SystemData &systemData);

    /**
     * Returns data field.
     */
    DataField getDataField(void);

    /**
     * @brief Puts the text into the video buffer.
     */
    void draw(void) const;
  };

  /**
   * @brief This class represents a page link.
   */
  class InteractiveLink {
  private:
    PageName mChildPage;
    TextLine mTextLine;

  public:
    /**
     * @brief Object constructor.
     */
    InteractiveLink(PageName childPage, TextLine textLine);

    /**
     * Default destructor.
     */
    ~InteractiveLink() = default;

    /**
     * @brief Returns pointer to the child page.
     */
    PageName getPage(void) const;

    /**
     * @brief Returns text of the page link.
     */
    TextLine getLine(void) const;

    /**
     * @brief Sets the child page pointer.
     */
    void setPage(PageName page);

    /**
     * @brief Sets the text of the link page.
     */
    void setText(TextLine textLine);

    /**
     * @brief Puts the text into the video buffer.
     */
    void draw(void) const;
  };

  /**
   * @brief Class that represents a single page in the menu system.
   */
  class Page {
  public:
    // Virtual methods
    virtual void addGraphic(std::unique_ptr<GraphicObject> graphic) = 0;
    virtual void draw(ext_spi_handle_t ext_spi) const = 0;
    virtual void updateDataAndParams(const SystemData &systemData) = 0;
    virtual void setSelection(uint8_t sel) = 0;
    virtual void changeSelection(bool increase) = 0;
    virtual void setCursor(void) const = 0;
    virtual ~Page() = default;
  };

  /**
   * @brief Class that represents a single menu page with page links in the menu system.
   */
  class LinkPage : public Page {
  private:
    std::vector<std::unique_ptr<GraphicObject>> mGraphic;
    std::vector<std::unique_ptr<InteractiveLink>> mLinks;
    uint8_t mSelected;

  public:
    /**
     * @brief Default constructor.
     */
    LinkPage() = default;

    /**
     * @brief Object constructor.
     */
    LinkPage(std::vector<std::unique_ptr<GraphicObject>> &&graphic, std::vector<std::unique_ptr<InteractiveLink>> &&links);

    /**
     * @brief Default destructor.
     */
    ~LinkPage() override = default;

    /**
     * @brief This function adds graphic object to the page.
     */
    void addGraphic(std::unique_ptr<GraphicObject> graphic) override;

    /**
     * @brief This function adds link to the page.
     */
    void addLink(std::unique_ptr<InteractiveLink> link);

    /**
     * @brief Returns a pointer to the child page.
     */
    PageName getChild(void);

    /**
     * @brief Updates data to display on the screen if present.
     */
    void updateDataAndParams(const SystemData &systemData) override;

    /**
     * @brief Sets selected link option.
     */
    void setSelection(uint8_t sel) override;

    /**
     * @brief Changes selected option to the next or the previos one depending on the argument.
     */
    void changeSelection(bool increase) override;

    /**
     * @brief Gets pointer to the selected link.
     */
    InteractiveLink *getSelectedLink(void);

    /**
     * @brief Sets cursor on the page if there are more than one options.
     */
    void setCursor(void) const override;

    /**
     * @brief Puts the graphic and interactive objects into the video buffer.
     */
    void draw(ext_spi_handle_t ext_spi) const override;
  };

  /**
   * @brief Class that represents a single menu page with parameters to change int the menu system.
   */
  class ParamPage : public Page {
  private:
    std::vector<std::unique_ptr<GraphicObject>> mGraphic;
    std::vector<std::unique_ptr<InteractiveParam>> mParams;
    uint8_t mSelected;
    PageName mParentPage;

  public:
    /**
     * @brief Default constructor.
     */
    ParamPage() = default;

    /**
     * @brief Object constructor.
     */
    ParamPage(std::vector<std::unique_ptr<GraphicObject>> &&graphic, std::vector<std::unique_ptr<InteractiveParam>> &&params);

    /**
     * @brief Default destructor.
     */
    ~ParamPage() override = default;

    /**
     * @brief This function adds graphic object to the page.
     */
    void addGraphic(std::unique_ptr<GraphicObject> graphic) override;

    /**
     * @brief This function adds param to the page.
     */
    void addParam(std::unique_ptr<InteractiveParam> param);

    /**
     * @brief Updates data and parameters to display on the screen if present.
     */
    void updateDataAndParams(const SystemData &systemData) override;

    /**
     * @brief Sets cursor on the page if there are more than one options.
     */
    void setCursor(void) const override;

    /**
     * @brief Returns a pointer to the child page.
     */
    PageName getParent(void);

    /**
     * @brief Returns selected option.
     */
    uint8_t getSelected(void);

    /**
     * @brief Returns params number.
     */
    uint8_t getParamsNum(void);

    /**
     * @brief Sets selected params option.
     */
    void setSelection(uint8_t sel) override;

    /**
     * @brief Changes selected option to the next or the previos one depending on the argument.
     */
    void changeSelection(bool increase) override;

    /**
     * @brief Gets pointer to the selected param.
     */
    InteractiveParam *getSelectedParam(void);

    /**
     * @brief Puts the graphic and interactive objects into the video buffer.
     */
    void draw(ext_spi_handle_t ext_spi) const override;
  };

} // namespace pedometer

#endif // MENU_PAGES_H
