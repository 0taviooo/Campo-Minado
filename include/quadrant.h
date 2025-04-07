#ifndef QUADRANT_H

#define QUADRANT_H

#include "utils.h"

struct Quadrant {
    Vector2 pos = {};
    float width = 0.0f;
    float height = 0.0f;
    bool active = false;
    bool boom = false;
    bool marked = false;
    vector <bool> count = {0, 0, 0, 0, 0, 0, 0, 0};
    Color color = BLANK;
    
    Quadrant (
        Vector2 pos_ = {},
        float width_ = 0.0f,
        float height_ = 0.0f,
        bool boom_ = false
    ):
    pos(pos_), width(width_), height(height_), boom(boom_) {}
    
    Rectangle QUADRANT = {pos.x, pos.y, width, height};
    
    int sum() {
        if (boom == false) {
            int s = 0;
            for (int i = 0; i < (int) count.size(); ++i) {
                s += count[i];
            }
            return s;
        }
        else {
            return -1;
        }
    }
    
    void show() {
        if (active == true) {
            switch (sum()) {
                case 0:
                    color = SKYBLUE;
                    break;
                case 1:
                    color = DARKBLUE;
                    break;
                case 2:
                    color = DARKGREEN;
                    break;
                case 3:
                    color = GOLD;
                    break;
                case 4:
                    color = ORANGE;
                    break;
                case 5:
                    color = VIOLET;
                    break;
                case 6:
                    color = DARKPURPLE;
                    break;
                case 7:
                    color = RED;
                    break;
                case 8:
                    color = MAROON;
                    break;
                default:
                    color = BLACK;
            }
        }
        DrawText(
            to_string(sum()).c_str(), Utils::centralize(MeasureText(to_string(sum()).c_str(), height / 2), {pos.x, pos.x + width}), pos.y + height / 3, height / 2, color
        );
    }
    
};

#endif