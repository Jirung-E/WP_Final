#include "TextBox.h"


TextBox::TextBox(const tstring& text, const Point& position, double width, double height) : text { text },
position { position }, width { width }, height { height }, bias { None },
transparent_background { false }, transparent_border { false },
square { true }, bold { 0 }, italic { false },
background_color { White }, text_color { Black }, 
border_color { White }, border_width { 1 }, align { DT_CENTER }, absolute { false } {

}


void TextBox::show(const HDC& hdc, const RECT& valid_area) const {
    drawBase(hdc, valid_area);
    drawText(hdc, valid_area);
}

RECT TextBox::absoluteArea(const RECT& valid_area) const {
    if(absolute) {
        return { LONG(position.x), LONG(position.y), LONG(position.x + width), LONG(position.y + height) };
    }

    RECT area = valid_area;
    if(square) {
        area = rectToSquare(valid_area);
        switch(bias) {
        case Left:
            area.right -= area.left - valid_area.left;
            area.left = valid_area.left;
            break;
        case Right:
            area.left += valid_area.right - area.right;
            area.right = valid_area.right;
            break;
        case Up:
            area.bottom -= area.top - valid_area.top;
            area.top = valid_area.top;
            break;
        case Down:
            area.top += valid_area.bottom - area.bottom;
            area.bottom = valid_area.bottom;
            break;
        }
    }
    int w = area.right - area.left;
    int h = area.bottom - area.top;
    int x = area.left + w * position.x / 100;
    int y = area.top + h * position.y / 100;
    return RECT { x, y, int(x + w * width / 100), int(y + h * height / 100) };
}


void TextBox::drawBase(const HDC& hdc, const RECT& valid_area) const {
    HBRUSH bg_br;
    if(transparent_background) {
        bg_br = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
    }
    else {
        bg_br = CreateSolidBrush(background_color);
    }
    int style = transparent_border ? PS_NULL : PS_SOLID;
    HPEN bg_pen = CreatePen(style, border_width, border_color);
    HBRUSH old_br = (HBRUSH)SelectObject(hdc, bg_br);
    HPEN old = (HPEN)SelectObject(hdc, bg_pen);

    RECT rect = absoluteArea(valid_area);
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    SelectObject(hdc, old_br);
    SelectObject(hdc, old);
    DeleteObject(bg_pen);
    DeleteObject(bg_br);
}

void TextBox::drawText(const HDC& hdc, const RECT& valid_area) const {
    LOGFONT logfont;
    GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &logfont);

    RECT rect = absoluteArea(valid_area);
    rect %= 90;

    SIZE size;
    GetTextExtentPoint32(hdc, text.c_str(), text.length(), &size);
    if(size.cx*height - size.cy*width > 0) {   // °¡·Î°¡ ´õ ±è
        logfont.lfHeight = (rect.bottom - rect.top) * 90 / 100 / size.cx * width;
        rect.top += ((rect.bottom - rect.top) * 90 / 100 - logfont.lfHeight) / 2;
    }
    else {      // ¼¼·Î°¡ ´õ ±è
        logfont.lfHeight = (rect.bottom - rect.top) * 90 / 100;
    }

    switch(bold) {
    case 1:
        logfont.lfWeight = FW_BOLD;
        break;
    case 2:
        logfont.lfWeight = FW_EXTRABOLD;
        break;
    case 3:
        logfont.lfWeight = FW_ULTRABOLD;
        break;
    case 4:
        logfont.lfWeight = FW_HEAVY;
        break;
    }
    logfont.lfItalic = italic;

    HFONT hNewFont = CreateFontIndirect(&logfont);
    HFONT old_font = (HFONT)SelectObject(hdc, hNewFont);

    COLORREF old_text_color = SetTextColor(hdc, text_color);
    COLORREF old_bk_color = SetBkColor(hdc, background_color);
    if(transparent_background) {
        SetBkMode(hdc, TRANSPARENT);
    }
    DrawText(hdc, text.c_str(), text.length(), &rect, align);

    SetTextColor(hdc, old_text_color);
    SetBkColor(hdc, old_bk_color);
    SelectObject(hdc, old_font);
    DeleteObject(hNewFont);
}