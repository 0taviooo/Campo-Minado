#ifndef BUTTON_H

#define BUTTON_H

#include "raylib.h"
#include "utils.h"
#include <cstring>

using namespace std;

struct Button {
    Vector2 pos = {0, 0};
    float border = 0.f;
    Vector2 gap = {0, 0};
    float size = 0.f;
    const char* content;
    Color color1 = BLANK;
    Color color2 = BLANK;
    Color color3 = BLANK;
    Rectangle BUTTON = {pos.x, pos.y, 0, 0};
    
    Button(
        float border_ = 0.f,
        Vector2 gap_ = {0, 0},
        float size_ = 0.f,
        const char* content_ = "",
        Color color1_ = BLANK,
        Color color2_ = BLANK,
        Color color3_ = BLANK
    ):
    border(border_), gap(gap_), size(size_), content(content_), color1(color1_), color2(color2_), color3(color3_) {}
    
    void update(Vector2 pos_) {
        pos = pos_;
        BUTTON = {pos.x + border, pos.y + border, get_width(false), get_height(false)};
    }
    float get_content_size() {
        return MeasureText(content, size);
    }
    float get_width(bool border_on = true) {
        float border_num = border_on ? border : 0;
        float width = get_content_size() + (border_num + gap.x) * 2;
        return width;
    }
    float get_height(bool border_on = true) {
        float border_num = border_on ? border : 0;
        float height = size + (border_num + gap.y) * 2;
        return height;
    }
    void draw() {
        Color sec_color = CheckCollisionPointRec(GetMousePosition(), BUTTON) ? color3 : color1;
        DrawRectangle(pos.x, pos.y, get_width(), get_height(), sec_color);
        DrawRectangleRec(BUTTON, color2);
        DrawText(content, pos.x + border + (get_width(false) - get_content_size()) / 2, pos.y + border + (get_height(false) - size) / 2, size, sec_color);
    }
    bool click() {
        return (CheckCollisionPointRec(GetMousePosition(), BUTTON) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
    }
};

#endif
