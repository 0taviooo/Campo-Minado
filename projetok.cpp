#include <iostream>
#include <raylib.h>
#include <vector>
#include <cmath>
#include <random>
#include <unordered_map>
#include <cstring>

using namespace std;
using UniformDist = std::uniform_int_distribution<int>;

struct Random {
    static int gen(int min, int max) {
        static mt19937 generator;
        uniform_int_distribution<int> distribution(min, max);
        int result = distribution(generator);
        return result;
    }
};

enum class State {
    startScreen, selectScreen, gameScreen, gameOverScreen, gameWinScreen
};

struct Constants {
    static const int windowX = 800;
    static const int windowY = 600;
};

bool timer(float currentTime, float& lastTime, float interval) {
    if (currentTime - lastTime > interval) {
        return true;
    }
    return false;
}

struct Quadrant {
    Vector2 pXY = {};
    float width = 0.0f;
    float height = 0.0f;
    bool active = false;
    bool boom = false;
    bool marked = false;
    vector <bool> count = {0, 0, 0, 0, 0, 0, 0, 0};
    Color color = BLANK;
    
    Quadrant (Vector2 pXY = {}, float width = 0.0f, float height = 0.0f, bool boom = false): pXY(pXY), width(width), height(height), boom(boom) {}
    Rectangle QUADRANT = {pXY.x, pXY.y, width, height};
    
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
        DrawText(to_string(sum()).c_str(), pXY.x + width / 5 * 2, pXY.y + height / 3, height / 2, color);
    }
    
};

struct Field {
    int size = 0;
    float width = 0.0f;
    float height = 0.0f;
    vector <Color> colors = {BLANK, BLANK, BLANK, BLANK};
    vector <Sound> sounds = {};
    Color mark = BLANK;
    vector <vector<Quadrant>> grid = {};
    int totalSpaces = 0;
    int viewSpaces = 0;
    Vector2 constructor = {};
    vector <bool> chances = {1, 0, 0, 0, 0};
    bool explosion = false;
    
    Field (int size = 0, float width = 0.0f, float height = 0.0f, vector <Color> colors = {BLANK, BLANK, BLANK, BLANK}, vector <Sound> sounds = {}, Color mark = BLANK): size(size), width(width), height(height), colors(colors), sounds(sounds), mark(mark) {}
    
    void construct() {
        totalSpaces = 0;
        viewSpaces = 0;
        for (int i = 0; i < size; ++i) {
            grid.push_back({});
            constructor.y = 0.0f;
            for (int j = 0; j < size; ++j) {
                grid[i].push_back({{0.0f + constructor.x, 0.0f + constructor.y}, width / size - 1, height / size - 1, chances[Random::gen(0, chances.size() - 1)]});
                constructor.y += height / size;
                if (grid[i][j].boom == false) ++totalSpaces;
            }
            constructor.x += width / size;
        }
    }
    void upgrade() {
        viewSpaces = 0;
        for (int i = 0; i < (int) grid.size(); ++i) {
            for (int j = 0; j < (int) grid[i].size(); ++j) {
                if (CheckCollisionPointRec(GetMousePosition(), grid[i][j].QUADRANT) &&  IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && grid[i][j].marked == false && grid[i][j].active == false) {
                    grid[i][j].active = true;
                    PlaySound(sounds[0]);
                    if (grid[i][j].boom == true) {
                        PlaySound(sounds[2]);
                        explosion = true;
                    }
                }
                if (CheckCollisionPointRec(GetMousePosition(), grid[i][j].QUADRANT) &&  IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && grid[i][j].active == false) {
                    if (grid[i][j].marked == false) grid[i][j].marked = true;
                    else { grid[i][j].marked = false; }
                    PlaySound(sounds[1]);
                }
                if (grid[i][j].active == true) {
                    ++viewSpaces;
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

struct Button {
    float height = 0.0f;
    float width = 0.0f;
    Color color1 = BLANK;
    Color color2 = BLANK;
    float border = 0.0f;

    Button(float height = 0.0f, float width = 0.0f, Color color1 = BLANK, Color color2 = BLANK, float border = 0.0f): height(height), width(width), color1(color1), color2(color2), border(border) {}
    Rectangle BUTTON = {0, 0, width, height};

    void draw(Vector2 pXY = {}, const string& content = "", float size = 0.0f, Color coloration = BLANK) {
        BUTTON = {pXY.x, pXY.y, width, height};
        DrawRectangle(pXY.x - border, pXY.y - border, width + border * 2, height + border * 2, color2);
        DrawRectangleRec(BUTTON, color1);
        DrawText(content.c_str(), pXY.x + width / 3, pXY.y + height / 3, size, coloration);
    }
    bool click() {
        return (CheckCollisionPointRec(GetMousePosition(), BUTTON) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT));
    }
};

void gameRun() {
    InitWindow(Constants::windowX, Constants::windowY, "Campo Minado");
    SetTargetFPS(60);
    
    float screenTime = GetTime();
    
    Color GRAY0 = {160, 160, 160, 255};
    Color GRAY1 = {150, 150, 150, 255};
    Color GRAY2 = {120, 120, 120, 255};
    Color GRAY3 = {180, 180, 180, 255};
    Color MARK = {185, 134, 134, 255};

    Texture2D shovelSprite = LoadTexture("images/shovel.png");
    Texture2D flagSprite = LoadTexture("images/flag.png");
    
    InitAudioDevice();
    
    Sound mus_swipe = LoadSound("audios/mus-sfx-swipe.mp3");
    Sound snd_switchpull = LoadSound("audios/snd-switchpull-n.mp3");
    Sound mus_explosion = LoadSound("audios/mus-explosion.mp3");
    Sound snd_buyitem = LoadSound("audios/snd-buyitem.mp3");
    Sound snd_dumbvictory = LoadSound("audios/snd-dumbvictory.mp3");
    Sound select_sound = LoadSound("audios/undertale-select-sound.mp3");
    Sound snd_escaped = LoadSound("audios/snd-escaped.mp3");
    
    int chronometer = 0;
    
    Field mineField = {5, Constants::windowX, Constants::windowY, {GRAY0, GRAY1, GRAY2, GRAY3}, {mus_swipe, snd_switchpull, mus_explosion}, MARK};
    
    State stage = State::startScreen;
    
    vector <Button> buttons = {};
    for (int i = 0; i < 3; ++i) {
        buttons.push_back({70, 200, BLACK, WHITE, 5});
    }
    vector <string> buttonsTitle = {"Easy", "Medium", "Hard"};
    
    while (!WindowShouldClose()) {
        if (stage == State::startScreen) {
            BeginDrawing();
            
            ClearBackground(WHITE);
                        
            if (timer(GetTime(), screenTime, 1.0f)) {
                DrawText("Happy Birthday! Press Enter to begin!", Constants::windowX / 32 * 4.5, Constants::windowY / 10 * 6, 30, BLACK);
                if (timer(GetTime(), screenTime, 2.0f)) screenTime = GetTime();
            }
            
            DrawText("I hope you like it ^^", Constants::windowX / 16 * 6.5, Constants::windowY / 16 * 15, 18, BLACK);
            
            EndDrawing();
            
            if (IsKeyPressed(KEY_ENTER)) {
                stage = State::selectScreen;
                PlaySound(snd_buyitem);
            }
        }
        if (stage == State::selectScreen) {
            BeginDrawing();
            
            ClearBackground(BLACK);
            
            DrawText("Choose your game!", Constants::windowX / 32 * 12, Constants::windowY / 10 * 3, 25, WHITE);
            
            for (int i = 0; i < (int) buttons.size(); ++i) {
                buttons[i].draw({(Constants::windowX / 16) + (buttons[i].width + buttons[i].border + 50) * i, Constants::windowY / 2}, buttonsTitle[i], 25, buttons[i].color2);
                if (buttons[i].click()) {
                    PlaySound(select_sound);
                    stage = State::gameScreen;
                    screenTime = GetTime();
                    chronometer = 0;
                }
            }
            
            EndDrawing();            
            
            if (buttons[0].click()) {
                mineField = {5, Constants::windowX, Constants::windowY - 100, {GRAY0, GRAY1, GRAY2, GRAY3}, {mus_swipe, snd_switchpull, mus_explosion}, MARK};
                mineField.construct();
                mineField.calc();
            };            
            if (buttons[1].click()) {
                mineField = {10, Constants::windowX, Constants::windowY - 100, {GRAY0, GRAY1, GRAY2, GRAY3}, {mus_swipe, snd_switchpull, mus_explosion}, MARK};
                mineField.construct();
                mineField.calc();
            };
            if (buttons[2].click()) {
                mineField = {20, Constants::windowX, Constants::windowY - 100, {GRAY0, GRAY1, GRAY2, GRAY3}, {mus_swipe, snd_switchpull, mus_explosion}, MARK};
                mineField.construct();
                mineField.calc();
            };

            
            if (IsKeyPressed(KEY_ENTER)) stage = State::gameScreen;
        }
        if (stage == State::gameScreen) {
            
            chronometer = (int) GetTime() - screenTime;
            
            mineField.upgrade();
            
            BeginDrawing();
        
            ClearBackground(BLACK);
        
            mineField.draw();
            
            DrawText(to_string(chronometer).c_str(), Constants::windowX / 32, Constants::windowY / 16 * 14.5, 40, WHITE);
            DrawTexture(shovelSprite, Constants::windowX / 6, Constants::windowY / 16 * 13.8, WHITE);
            DrawText("Left Mouse Button", Constants::windowX / 8, Constants::windowY / 64 * 63.9, 15, WHITE);
            DrawTexture(flagSprite, Constants::windowX / 10 * 4.5, Constants::windowY / 16 * 13.8, WHITE);
            DrawText("Right Mouse Button", Constants::windowX / 10 * 4, Constants::windowY / 64 * 63.9, 15, WHITE);
            DrawText("Backspace to quit", Constants::windowX / 10 * 7, Constants::windowY / 32 * 30, 20, WHITE);
        
            EndDrawing();
            
            if (IsKeyPressed(KEY_BACKSPACE)) {
                stage = State::selectScreen;
                PlaySound(snd_escaped);
            }
            if (mineField.explosion == true) stage = State::gameOverScreen;
            if (mineField.viewSpaces >= mineField.totalSpaces) {
                PlaySound(snd_dumbvictory);
                stage = State::gameWinScreen;
            }
        }
        if (stage == State::gameOverScreen) {
            BeginDrawing();
            
            ClearBackground(BLACK);
            
            DrawText("Game Over!", Constants::windowX / 5, Constants::windowY / 4, 90, WHITE);
            
            if (timer(GetTime(), screenTime, 1.0f)) {
                DrawText("Don't give up! Press Enter to return!", Constants::windowX / 32 * 7, Constants::windowY / 10 * 4.4, 25, WHITE);
                if (timer(GetTime(), screenTime, 2.0f)) screenTime = GetTime();
            }
            
            EndDrawing();
            
            if (IsKeyPressed(KEY_ENTER)) {
                stage = State::selectScreen;
                mineField.explosion = false;
            }
        }
        if (stage == State::gameWinScreen) {
            BeginDrawing();
            
            ClearBackground(BLACK);
            
            DrawText("Congratulations!", Constants::windowX / 26, Constants::windowY / 4, 90, WHITE);
            if (timer(GetTime(), screenTime, 1.0f)) {
                DrawText("I hope you've enjoyed this play! Press Enter to return!", Constants::windowX / 32 * 2.5, Constants::windowY / 10 * 4.4, 25, WHITE);
                if (timer(GetTime(), screenTime, 2.0f)) screenTime = GetTime();
            }
            DrawText(("You made it in " + to_string(chronometer) + " seconds!").c_str(), Constants::windowX / 3.5, Constants::windowY / 32 * 6, 30, WHITE);
            
            EndDrawing();
            
            if (IsKeyPressed(KEY_ENTER)) stage = State::selectScreen;
        }
    }
    CloseWindow();
    
    CloseAudioDevice();
    
    UnloadSound(mus_swipe);
    UnloadSound(snd_switchpull);
    UnloadSound(snd_buyitem);
    UnloadSound(mus_explosion);
    UnloadSound(snd_dumbvictory);
    UnloadSound(select_sound);
    UnloadSound(snd_escaped);
}

int main() {

    gameRun();

    return 0;
}