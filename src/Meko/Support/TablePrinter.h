#ifndef SUPPORT_TABLEPRINTER_H
#define SUPPORT_TABLEPRINTER_H

#include "../Meko.h"

namespace Meko::Support {
class TablePrinterStyle {
   public:
    typedef std::unordered_map<raw_str, raw_str> styleDef;
    const static styleDef empty;
    const static styleDef simple;
    const static styleDef thin;
    const static styleDef thick;
    TablePrinterStyle(bool headline = true, bool grid = false, const styleDef& frameCharSet = thin);
    virtual ~TablePrinterStyle();
    /**
     * render the first line of the table
     */
    virtual raw_str renderOpening();
    /**
     * render before the first data of the row is rendered
     */
    virtual raw_str renderFirst();
    /**
     * render between data outputs
     */
    virtual raw_str renderBetween(size_t i);
    /**
     * render after the last data of the row is rendered
     */
    virtual raw_str renderLast();
    /**
     * render the last line
     */
    virtual raw_str renderClosing();

    void setData(const std::vector<AutoPadder> padders, size_t rows);

    virtual raw_str renderHorizontal(raw_str_arg leftBorder, raw_str_arg vLine, raw_str_arg cross, raw_str_arg rightBorder);

   protected:
    bool headline;
    bool grid;
    std::vector<AutoPadder> padders;
    size_t rows;
    size_t columns;
    size_t cRow = 0;
    styleDef borders;
    raw_str stretch(raw_str pattern, size_t length);
};
class TablePrinter : public Singleton<TablePrinter> {
   public:
    typedef raw_str dataType;
    typedef std::vector<dataType> rowType;
    typedef std::vector<rowType> sheetType;

    typedef const dataType& dataRef;
    typedef const rowType& rowRef;
    typedef const sheetType& sheetRef;
    /**
     * print an aligned table of values
     * 
     * when not stated otherwise, the provided style is being consumed
     * so you can pass "new StyleImplementation" without thinking about deleting the style after usage
     * pass false as the last parameter if you wish to reuse the style class
     */
    raw_str render(sheetRef data, TablePrinterStyle* style = nullptr, bool consumeStyle = true);
    std::vector<AutoPadder> initializePadders(sheetRef data);
};
}  // namespace Meko::Support
#endif
