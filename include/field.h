#ifndef FIELD_H

#define FIELD_H

#include "quadrant.h"

struct Field {
    int size = 0;
    float width = 0.0f;
    float height = 0.0f;
    vector <Color> colors = {BLANK, BLANK, BLANK, BLANK};
    vector <Sound*> sounds = {};
    Color mark = BLANK;
    vector <vector<Quadrant>> grid = {};
    int bomb_spaces = 0;
    int active_spaces = 0;
    int all_spaces = 0;
    Vector2 constructor = {};
    vector <bool> chances = {1, 0, 0, 0, 0};
    bool explosion = false;
    bool victory = false;
    
    Field(
        int size_ = 0,
        float width_ = 0.0f,
        float height_ = 0.0f,
        vector <Color> colors_ = {BLANK, BLANK, BLANK, BLANK},
        vector <Sound*> sounds_ = {},
        Color mark_ = BLANK
    ):
    size(size_), width(width_), height(height_), colors(colors_), sounds(sounds_), mark(mark_) {
        all_spaces = size * size;
    }
    
    void construct() {
        bomb_spaces = 0;
        active_spaces = 0;

        for (int i = 0; i < size; ++i) {
            grid.push_back({});
            constructor.y = 0.0f;
            for (int j = 0; j < size; ++j) {
                grid[i].push_back(
                    {{constructor.x, constructor.y}, width / size - 1, height / size - 1, chances[Random::gen(0, chances.size() - 1)]}
                );
                constructor.y += height / size;
                if (grid[i][j].boom == true) ++bomb_spaces;
            }
            constructor.x += width / size;
        }
    }
    void upgrade() {
        // viewSpaces = 0;
        for (int i = 0; i < (int) grid.size(); ++i) {
            for (int j = 0; j < (int) grid[i].size(); ++j) {
                // Se clilcar com o botão esquerdo do mouse
                if (CheckCollisionPointRec(GetMousePosition(), grid[i][j].QUADRANT) &&  IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && grid[i][j].marked == false && grid[i][j].active == false) {
                    // Se não houver qualquer espaço visível
                    if (active_spaces == 0) {
                        grid[i][j].boom = false;
                        --bomb_spaces;
                        calc();
                    }
                    grid[i][j].active = true;
                    PlaySound(*sounds[0]);
                    // Se o quadrante escolhido for uma bomba
                    if (grid[i][j].boom == true) {
                        PlaySound(*sounds[2]);
                        explosion = true;
                    }
                }
                // Se clilcar com o botão direito do mouse
                if (CheckCollisionPointRec(GetMousePosition(), grid[i][j].QUADRANT) &&  IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && grid[i][j].active == false) {
                    if (grid[i][j].marked == false) grid[i][j].marked = true;
                    else { grid[i][j].marked = false; }
                    PlaySound(*sounds[1]);
                }
                
                // Se o quadrante ativo for livre, os outros quadrantes livres adjacentes irão se ativar
                if (grid[i][j].active == true) {
                    grid[i][j].marked = false;
                    if (!(i + 1 > size-1)) {
                        if (grid[i+1][j].sum() == 0) {
                            grid[i+1][j].active = true;
                            if (!(i + 1 > size-1)) {
                                if (grid[i+1][j].sum() != -1) grid[i+1][j].active = true;
                            }
                            if (!(i - 1 < 0)) {
                                if (grid[i-1][j].sum() != -1) grid[i-1][j].active = true;
                            }
                            if (!(j + 1 > size-1)) {
                                if (grid[i][j+1].sum() != -1) grid[i][j+1].active = true;
                            }
                            if (!(j - 1 < 0)) {
                                if (grid[i][j-1].sum() != -1) grid[i][j-1].active = true;
                            }
                        }
                    }
                    if (!(i - 1 < 0)) {
                        if (grid[i-1][j].sum() == 0) {
                            grid[i-1][j].active = true;
                            if (!(i + 1 > size-1)) {
                                if (grid[i+1][j].sum() != -1) grid[i+1][j].active = true;
                            }
                            if (!(i - 1 < 0)) {
                                if (grid[i-1][j].sum() != -1) grid[i-1][j].active = true;
                            }
                            if (!(j + 1 > size-1)) {
                                if (grid[i][j+1].sum() != -1) grid[i][j+1].active = true;
                            }
                            if (!(j - 1 < 0)) {
                                if (grid[i][j-1].sum() != -1) grid[i][j-1].active = true;
                            }
                        }
                    }
                    if (!(j + 1 > size-1)) {
                        if (grid[i][j+1].sum() == 0) {
                            grid[i][j+1].active = true;
                            if (!(i + 1 > size-1)) {
                                if (grid[i+1][j].sum() != -1) grid[i+1][j].active = true;
                            }
                            if (!(i - 1 < 0)) {
                                if (grid[i-1][j].sum() != -1) grid[i-1][j].active = true;
                            }
                            if (!(j + 1 > size-1)) {
                                if (grid[i][j+1].sum() != -1) grid[i][j+1].active = true;
                            }
                            if (!(j - 1 < 0)) {
                                if (grid[i][j-1].sum() != -1) grid[i][j-1].active = true;
                            }
                        }
                    }
                    if (!(j - 1 < 0)) {
                        if (grid[i][j-1].sum() == 0) {
                            grid[i][j-1].active = true;
                            if (!(i + 1 > size-1)) {
                                if (grid[i+1][j].sum() != -1) grid[i+1][j].active = true;
                            }
                            if (!(i - 1 < 0)) {
                                if (grid[i-1][j].sum() != -1) grid[i-1][j].active = true;
                            }
                            if (!(j + 1 > size-1)) {
                                if (grid[i][j+1].sum() != -1) grid[i][j+1].active = true;
                            }
                            if (!(j - 1 < 0)) {
                                if (grid[i][j-1].sum() != -1) grid[i][j-1].active = true;
                            }
                        }
                    }
                }
            }
        }
    }
    // Calcula as bombas próximas de cada quadrante
    void calc() {
        for (int i = 0; i < (int) grid.size(); ++i) {
            for (int j = 0; j < (int) grid[i].size(); ++j) {
                if (i - 1 < 0){
                    if (j - 1 < 0) {
                        grid[i][j].count = {false, false, false, false, grid[i+1][j].boom, false, grid[i][j+1].boom, grid[i+1][j+1].boom};
                    }
                    else if (j + 1 > size-1) {
                        grid[i][j].count = {false, grid[i][j-1].boom, grid[i+1][j-1].boom, false, grid[i+1][j].boom, false, false, false};
                    }
                    else {
                        grid[i][j].count = {false, grid[i][j-1].boom, grid[i+1][j-1].boom, false, grid[i+1][j].boom, false, grid[i][j+1].boom, grid[i+1][j+1].boom};
                    }
                }
                else if (i + 1 > size-1) {
                    if (j + 1 > size-1) {
                        grid[i][j].count = {grid[i-1][j-1].boom, grid[i][j-1].boom, false, grid[i-1][j].boom, false, false, false, false};
                    }
                    else if (j - 1 < 0) {
                        grid[i][j].count = {false, false, false, grid[i-1][j].boom, false, grid[i-1][j+1].boom, grid[i][j+1].boom, false};
                    }
                    else {
                        grid[i][j].count = {grid[i-1][j-1].boom, grid[i][j-1].boom, false, grid[i-1][j].boom, false, grid[i-1][j+1].boom, grid[i][j+1].boom, false};
                    }
                }
                else if (j - 1 < 0) {
                    grid[i][j].count = {false, false, false, grid[i-1][j].boom, grid[i+1][j].boom, grid[i-1][j+1].boom, grid[i][j+1].boom, grid[i+1][j+1].boom};
                }
                else if (j + 1 > size-1) {
                    grid[i][j].count = {grid[i-1][j-1].boom, grid[i][j-1].boom, grid[i+1][j-1].boom, grid[i-1][j].boom, grid[i+1][j].boom, false, false, false};
                }
                else {
                    grid[i][j].count = {grid[i-1][j-1].boom, grid[i][j-1].boom, grid[i+1][j-1].boom, grid[i-1][j].boom, grid[i+1][j].boom, grid[i-1][j+1].boom, grid[i][j+1].boom, grid[i+1][j+1].boom};
                }
            }
        }
    }

    void check_vitory() {
        int counter = 0;
        for (vector<Quadrant> i: grid) {
            for (Quadrant j: i) {
                if (j.active == true && j.boom == false) ++counter;
            }
        }
        if (counter >= all_spaces - bomb_spaces) victory = true;
    }

    void set_activate_spaces() {
        int counter = 0;
        for (vector<Quadrant> i: grid) {
            for (Quadrant j: i) {
                if (j.active == true) ++counter;
            }
        }
        active_spaces = counter;
    }

    void draw() {
        for (int i = 0; i < (int) grid.size(); ++i) {
            for (int j = 0; j < (int) grid[i].size(); ++j) {
                if (grid[i][j].marked == false) {
                    if (CheckCollisionPointRec(GetMousePosition(), grid[i][j].QUADRANT) && grid[i][j].active == false) {
                        DrawRectangleRec(grid[i][j].QUADRANT, colors[2]);
                    }
                    else if (grid[i][j].active == false) {
                        if (i % 2 == 0) {
                            if (j % 2 == 0) {
                                DrawRectangleRec(grid[i][j].QUADRANT, colors[0]);
                            }
                            else {
                                DrawRectangleRec(grid[i][j].QUADRANT, colors[1]);
                            }
                        }
                        else {
                            if (j % 2 == 0) {
                                DrawRectangleRec(grid[i][j].QUADRANT, colors[1]);
                            }
                            else {
                                DrawRectangleRec(grid[i][j].QUADRANT, colors[0]);
                            }
                        }
                    }
                    else {
                        DrawRectangleRec(grid[i][j].QUADRANT, colors[3]);
                    }
                }
                else {
                    DrawRectangleRec(grid[i][j].QUADRANT, mark);
                }
                grid[i][j].show();
            }
        }
    }
};

#endif