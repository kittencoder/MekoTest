#include "../Meko.h"
namespace Meko::Support {
const TablePrinterStyle::styleDef TablePrinterStyle::empty = {
    {"TopLeft", " "},
    {"TopRight", " "},
    {"BottomLeft", " "},
    {"BottomRight", " "},

    {"Top", " "},
    {"Right", " "},
    {"Left", " "},
    {"Bottom", " "},
    {"InV", " "},
    {"InH", " "},

    {"TopBranch", " "},
    {"RightBranch", " "},
    {"LeftBranch", " "},
    {"BottomBranch", " "},

    {"InBranch", " "},
};
const TablePrinterStyle::styleDef TablePrinterStyle::simple = {
    {"TopLeft", "/"},
    {"TopRight", "\\"},
    {"BottomLeft", "\\"},
    {"BottomRight", "/"},

    {"Top", "-"},
    {"Right", "|"},
    {"Left", "|"},
    {"Bottom", "-"},
    {"InV", "|"},
    {"InH", "-"},

    {"TopBranch", "v"},
    {"RightBranch", "<"},
    {"LeftBranch", ">"},
    {"BottomBranch", "^"},

    {"InBranch", "+"},
};
const TablePrinterStyle::styleDef TablePrinterStyle::thin = {
    {"TopLeft", "┌"},
    {"TopRight", "┐"},
    {"BottomLeft", "└"},
    {"BottomRight", "┘"},

    {"Top", "─"},
    {"Right", "│"},
    {"Left", "│"},
    {"Bottom", "─"},
    {"InV", "│"},
    {"InH", "─"},

    {"TopBranch", "┬"},
    {"RightBranch", "┤"},
    {"LeftBranch", "├"},
    {"BottomBranch", "┴"},

    {"InBranch", "┼"},
};
const TablePrinterStyle::styleDef TablePrinterStyle::thick = {
    {"TopLeft", "┏"},
    {"TopRight", "┓"},
    {"BottomLeft", "┗"},
    {"BottomRight", "┛"},

    {"Top", "━"},
    {"Right", "┃"},
    {"Left", "┃"},
    {"Bottom", "━"},
    {"InV", "┃"},
    {"InH", "━"},

    {"TopBranch", "┳"},
    {"RightBranch", "┫"},
    {"LeftBranch", "┣"},
    {"BottomBranch", "┻"},

    {"InBranch", "╋"},
};
TablePrinterStyle::TablePrinterStyle(bool headline, bool grid, const styleDef& frameCharSet) : headline(headline), grid(grid) {
    for (const auto& i : empty) {
        raw_str_arg n = i.first;
        // get style with fallback to empty
        if (frameCharSet.find(n) == frameCharSet.end()) {
            borders[n] = empty.at(n);
        } else {
            borders[n] = frameCharSet.at(n);
        }
    }
}
TablePrinterStyle::~TablePrinterStyle() {
}
void TablePrinterStyle::setData(const std::vector<AutoPadder> padders, size_t rows) {
    this->padders = padders;
    this->rows = rows;
    this->columns = padders.size();
}
raw_str TablePrinterStyle::stretch(raw_str pattern, size_t length) {
    raw_str s;
    for (size_t i = 0; i < length; ++i) {
        s += pattern;
    }
    return s;
}
raw_str TablePrinterStyle::renderOpening() {
    return renderHorizontal(borders["TopLeft"], borders["Top"], borders["TopBranch"], borders["TopRight"]);
}
raw_str TablePrinterStyle::renderFirst() {
    return borders["Left"];
}
raw_str TablePrinterStyle::renderBetween(size_t i) {
    NOT_USED(i);
    return borders["InV"];
}
raw_str TablePrinterStyle::renderLast() {
    raw_str r = borders["Right"] + "\n";
    if ((grid && cRow != rows - 1) || (headline && cRow == 0)) {
        r += renderHorizontal(borders["LeftBranch"], borders["InH"], borders["InBranch"], borders["RightBranch"]);
    }
    ++cRow;
    return r;
}
raw_str TablePrinterStyle::renderHorizontal(raw_str_arg leftBorder, raw_str_arg vLine, raw_str_arg cross, raw_str_arg rightBorder) {
    raw_str s = leftBorder;
    bool first = true;
    for (const AutoPadder& p : padders) {
        if (first) {
            first = false;
        } else {
            s += cross;
        }
        // account for an inserted space before each vale
        s += stretch(vLine, p.getWidth() + 1);
    }
    return s + rightBorder + "\n";
}
raw_str TablePrinterStyle::renderClosing() {
    return renderHorizontal(borders["BottomLeft"], borders["Bottom"], borders["BottomBranch"], borders["BottomRight"]);
}
raw_str TablePrinter::render(sheetRef data, TablePrinterStyle* s, bool consumeStyle) {
    bool defStyle = s == nullptr;
    if (defStyle) {
        s = new TablePrinterStyle();
    }
    TablePrinterStyle& style = *s;
    std::vector<AutoPadder> padders = initializePadders(data);
    style.setData(padders, data.size());
    raw_str result = style.renderOpening();
    for (rowRef row : data) {
        result += style.renderFirst();
        for (size_t i = 0; i < padders.size(); ++i) {
            if (i != 0) {
                result += style.renderBetween(i);
            }
            // compensate shorter rows
            if (i >= row.size()) {
                result += " " + padders[i]("");
            } else {
                result += " " + padders[i](row[i]);
            }
        }
        result += style.renderLast();
    }
    result += style.renderClosing();
    if (defStyle) {
        delete s;
    } else if (consumeStyle) {
        delete s;
    }
    return result;
}
std::vector<AutoPadder> TablePrinter::initializePadders(sheetRef data) {
    std::vector<AutoPadder> padders;
    for (rowRef row : data) {
        if (row.size() > padders.size()) {
            padders.resize(row.size());
        }
        for (size_t i = 0; i < row.size(); ++i) {
            padders[i].show(row[i] + " ");
        }
    }
    return padders;
}
}  // namespace Meko::Support
