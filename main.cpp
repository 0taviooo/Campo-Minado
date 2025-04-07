#include <iostream>
#include <raylib.h>
#include <vector>
#include <ctime>
#include <cmath>
#include <string>
#include <unordered_map>

#include "include/constants.h"
#include "include/game-state.h"
#include "include/utils.h"
#include "include/resource.h"
#include "include/random.h"
#include "include/button.h"
#include "include/field.h"

using namespace std;

struct Game {
    GameState currentState = GameState::MainTitleScreen;
    int difficulty = 0;
    vector<int> score = {0, 0, 0};
    vector<int> high_score = {0, 0, 0};
    Texture2D shovel_sprite;
    Texture2D flag_sprite;
    Field field;
    
    Game(Resources& resources);
    static void Initialize();
    
    void playMusic(Resources& resources, const char* title);
    
    void mainTitleScreen(Resources& resources, Color startColor);
    void selectionScreen(Resources& resources);
    void playingScreen(Resources& resources, bool change, bool toggle, Field field, int points);
    void gameOverScreen(Resources& resources, int score, Color startColor, int best_time, bool new_record, bool explosion);
    
    
    struct MainTitleScreenData {
        static constexpr const char* label_t1 = "Campo Minado";
        static constexpr const char* label_t2 = "Pressione Enter para começar!";
        static constexpr const char* label_t3 = "Descoupado ltda";
    };
    
    struct SelectionScreenData {
        static constexpr const char* label_t1 = "Selecione a dificuldade:";
        static const vector<const char*> labels;
        
        static vector<Button> buttons;
        
        static vector<Button> generateButtons();
    };    
    
    struct PlayingScreenData {
        static constexpr const char* label_t1 = "Clique Esquerdo";
        static constexpr const char* label_t2 = "Clique Direito";
        static constexpr const char* label_t3 = "Backspace \npara sair";
    };
    
    struct GameOverScreenData {
        static constexpr const char* label_t1 = "Fim de Jogo!";
        static constexpr const char* label_t2 = "Pontuação: ";
        static constexpr const char* label_t3 = "Novo recorde!";
        static constexpr const char* label_t4 = "Recorde: ";
        static constexpr const char* label_t5 = "Não desista!";
        static constexpr const char* label_t6 = "Pressione Enter para retornar!";
    };
};

Game::Game(Resources& resources) {
    Initialize();
}

vector<Button> Game::SelectionScreenData::buttons = Game::SelectionScreenData::generateButtons();
void Game::Initialize() {
    Game::SelectionScreenData::buttons = Game::SelectionScreenData::generateButtons();
}
const vector<const char*> Game::SelectionScreenData::labels = {
    "Fácil",
    "Médio",
    "Difícil"
};
vector<Button> Game::SelectionScreenData::generateButtons() {
    vector<Button> v = {};
    for (unsigned int i = 0; i < labels.size(); ++i) {
        v.push_back({5.f, {20, 15}, FontSize::h2, labels[i], WHITE, BLACK, GRAY});
        v[i].update({Utils::centralize(v[i].get_width(), {0, GameConstants::windowX}), GameConstants::windowYPieces[i * 2 + 4]});
    }
    return v;
}

void Game::playMusic(Resources& resources, const char* title) {
    UpdateMusicStream(resources.musics.at(title));
}

void Game::mainTitleScreen(Resources& resources, Color startColor) {
    Game::playMusic(resources, "main_title_music");
    
    DrawText(Game::MainTitleScreenData::label_t1, Utils::centralize(MeasureText(Game::MainTitleScreenData::label_t1, FontSize::h1), {0, GameConstants::windowX}), GameConstants::windowYPieces[3], FontSize::h1, WHITE);
    DrawText(Game::MainTitleScreenData::label_t2, Utils::centralize(MeasureText(Game::MainTitleScreenData::label_t2, FontSize::h2), {0, GameConstants::windowX}), GameConstants::windowYPieces[6], FontSize::h2, startColor);
    DrawText(Game::MainTitleScreenData::label_t3, Utils::centralize(MeasureText(Game::MainTitleScreenData::label_t3, FontSize::note), {0, GameConstants::windowX}), GameConstants::windowYPieces[9], FontSize::note, WHITE);
}

void Game::selectionScreen(Resources& resources) {
    Game::playMusic(resources, "menu_music");
    
    DrawText(Game::SelectionScreenData::label_t1, Utils::centralize(MeasureText(Game::SelectionScreenData::label_t1, FontSize::h2), {0, GameConstants::windowX}), GameConstants::windowYPieces[3], FontSize::h2, WHITE);
    for (Button button: Game::SelectionScreenData::buttons) {
        button.draw();
    }
}

void Game::playingScreen(Resources& resources, bool change, bool toggle, Field field, int points)  {
    Game::playMusic(resources, "playing_music");

    DrawText(to_string(points).c_str(), GameConstants::windowXPieces[1], GameConstants::windowYPieces[10]-30, FontSize::h1, WHITE);
    
    field.draw();

    Game::shovel_sprite = resources.textures["shovel_sprite"];
    Game::flag_sprite = resources.textures["flag_sprite"];
    
    DrawTexture(shovel_sprite, Utils::centralize(48, {GameConstants::windowXPieces[1], GameConstants::windowXPieces[4]}), GameConstants::windowYPieces[9]+20, WHITE);
    DrawTexture(flag_sprite, Utils::centralize(48, {GameConstants::windowXPieces[3], GameConstants::windowXPieces[9]}), GameConstants::windowYPieces[9]+20, WHITE);
        
    DrawText(Game::PlayingScreenData::label_t1, GameConstants::windowXPieces[3], GameConstants::windowYPieces[10]-10, FontSize::body, WHITE);
    DrawText(Game::PlayingScreenData::label_t2, GameConstants::windowXPieces[7], GameConstants::windowYPieces[10]-10, FontSize::body, WHITE);

    DrawText(Game::PlayingScreenData::label_t3, GameConstants::windowXPieces[10], GameConstants::windowYPieces[10]-20, FontSize::body, WHITE);
}

void Game::gameOverScreen(Resources& resources, int score, Color startColor, int best_time, bool new_record, bool explosion) {
    
    DrawText(Game::GameOverScreenData::label_t1, Utils::centralize(MeasureText(Game::GameOverScreenData::label_t1, FontSize::h1), {0, GameConstants::windowX}), GameConstants::windowYPieces[3], FontSize::h1, WHITE);
    
    if (!explosion) {
        Game::playMusic(resources, "game_over_music");

        DrawText((string(Game::GameOverScreenData::label_t2) + to_string(score)).c_str(), Utils::centralize(MeasureText((string(Game::GameOverScreenData::label_t2) + to_string(score)).c_str(), FontSize::body), {0, GameConstants::windowX}), GameConstants::windowYPieces[4], FontSize::body, WHITE);
        
        if (new_record) DrawText(Game::GameOverScreenData::label_t3, Utils::centralize(MeasureText(Game::GameOverScreenData::label_t3, FontSize::body), {0, GameConstants::windowX}), GameConstants::windowYPieces[5], FontSize::body, startColor);
        else {
            DrawText(string((Game::GameOverScreenData::label_t4) + to_string(best_time)).c_str(), Utils::centralize(MeasureText(string((Game::GameOverScreenData::label_t4) + to_string(best_time)).c_str(), FontSize::body), {0, GameConstants::windowX}), GameConstants::windowYPieces[5], FontSize::body, startColor);
        }
    }
    if (explosion) {
        DrawText(Game::GameOverScreenData::label_t5, Utils::centralize(MeasureText(Game::GameOverScreenData::label_t5, FontSize::h2), {0, GameConstants::windowX}), GameConstants::windowYPieces[6], FontSize::h2, WHITE);
    }
    DrawText(Game::GameOverScreenData::label_t6, Utils::centralize(MeasureText(Game::GameOverScreenData::label_t6, FontSize::h2), {0, GameConstants::windowX}), GameConstants::windowYPieces[7], FontSize::h2, startColor);
}

int main() {
    InitWindow(GameConstants::windowX, GameConstants::windowY, "Campo Minado");
    
    const int fps = 60;
    SetTargetFPS(fps);
    
    InitAudioDevice();
    Resources* resources = new Resources();
    Game game = {*resources};
    
    bool changeColor = false;
    bool toggle = false;
    bool toggle_two = false;
    bool new_record = false;
    
    Color GRAY0 = {160, 160, 160, 255};
    Color GRAY1 = {150, 150, 150, 255};
    Color GRAY2 = {120, 120, 120, 255};
    Color GRAY3 = {180, 180, 180, 255};
    Color MARK = {185, 134, 134, 255};
    
    float start = GetTime();
    int counter = 0;
    int chronometer = 0;
    float chronometer_correction = 0;
    
    PlayMusicStream(resources->musics.at("main_title_music"));

    while (!WindowShouldClose()) {
        
        BeginDrawing();
        
        ClearBackground(BLACK);
        
        if (game.currentState == GameState::MainTitleScreen) {
            changeColor = Utils::timer(GetTime(), start, GameConstants::blinkIterval);
            if (changeColor) {
                ++counter;
                toggle = counter % 2 == 0;
            }
            
            game.mainTitleScreen(*resources, toggle ? WHITE : BLACK);
            
            if (IsKeyPressed(KEY_ENTER)) {
                PlaySound(resources->sounds.at("select_sound"));
                PlayMusicStream(resources->musics.at("menu_music"));
                game.currentState = GameState::SelectionScreen;
            }
        }
        else if (game.currentState == GameState::SelectionScreen) {
            game.selectionScreen(*resources);
            if (Game::SelectionScreenData::buttons[0].click()) {
                PlaySound(resources->sounds.at("select_sound"));
                game.field = {5, GameConstants::windowX, GameConstants::windowY - 120, {GRAY0, GRAY1, GRAY2, GRAY3}, {&resources->sounds.at("swipe_sound"), &resources->sounds.at("switchpull_sound"), &resources->sounds.at("explosion_sound")}, MARK};
                game.currentState = GameState::PlayingScreen;
            }
            else if (Game::SelectionScreenData::buttons[1].click()) {
                PlaySound(resources->sounds.at("select_sound"));
                game.field = {10, GameConstants::windowX, GameConstants::windowY - 120, {GRAY0, GRAY1, GRAY2, GRAY3}, {&resources->sounds.at("swipe_sound"), &resources->sounds.at("switchpull_sound"), &resources->sounds.at("explosion_sound")}, MARK};
                game.currentState = GameState::PlayingScreen;
            }
            else if (Game::SelectionScreenData::buttons[2].click()) {
                PlaySound(resources->sounds.at("select_sound"));
                game.field = {15, GameConstants::windowX, GameConstants::windowY - 120, {GRAY0, GRAY1, GRAY2, GRAY3}, {&resources->sounds.at("swipe_sound"), &resources->sounds.at("switchpull_sound"), &resources->sounds.at("explosion_sound")}, MARK};
                game.currentState = GameState::PlayingScreen;
            }
            if (game.currentState == GameState::PlayingScreen) {
                game.field.construct();
                game.field.calc();
                SeekMusicStream(resources->musics.at("menu_music"), 0.f);
                PlayMusicStream(resources->musics.at("playing_music"));

                chronometer = 0;
                chronometer_correction = GetTime();
            }
        }
        else if (game.currentState == GameState::PlayingScreen) {
            game.field.upgrade();
            game.field.check_vitory();
            game.field.set_activate_spaces();

            if (Utils::timer(GetTime(), chronometer_correction, 1)) {
                ++chronometer;
            }

            game.score[game.difficulty] = chronometer;

            if (IsKeyPressed(KEY_BACKSPACE)) {
                game.currentState = GameState::SelectionScreen;
                SeekMusicStream(resources->musics.at("playing_music"), 0.f);
                PlaySound(resources->sounds.at("escaped_sound"));
                PlayMusicStream(resources->musics.at("menu_music"));
            }
            if (game.field.explosion == true) {
                game.currentState = GameState::GameOverScreen;
                SeekMusicStream(resources->musics.at("playing_music"), 0.f);
                PlayMusicStream(resources->musics.at("game_over_music"));
                new_record = game.score[game.difficulty] > game.high_score[game.difficulty];
                if (new_record) game.high_score[game.difficulty] = game.score[game.difficulty];
            }

            if (game.field.victory == true) {
                SeekMusicStream(resources->musics.at("playing_music"), 0.f);
                PlaySound(resources->sounds.at("dumbvictory_sound"));
                PlayMusicStream(resources->musics.at("game_over_music"));
                game.currentState = GameState::GameOverScreen;
            }

            if (game.currentState == GameState::GameOverScreen && game.field.explosion == false) {
                if (game.score[game.difficulty] != 0 && game.score[game.difficulty] < game.high_score[game.difficulty]) {
                    game.high_score[game.difficulty] = game.score[game.difficulty];
                    new_record = true;
                }
            }
            
            game.playingScreen(*resources, toggle, toggle_two, game.field, game.score[game.difficulty]);
        }
        else if (game.currentState == GameState::GameOverScreen) {
            changeColor = Utils::timer(GetTime(), start, GameConstants::blinkIterval);
            if (changeColor) {
                ++counter;
                toggle = counter % 2 == 0;
            }
            
            game.gameOverScreen(*resources, game.score[game.difficulty], toggle ? WHITE : BLACK, game.high_score[game.difficulty], new_record, game.field.explosion);
            
            if (IsKeyPressed(KEY_ENTER)) {
                PlaySound(resources->sounds.at("select_sound"));
                SeekMusicStream(resources->musics.at("game_over_music"), 0.f);
                PlayMusicStream(resources->musics.at("playing_music"));
                game.score[game.difficulty] = 0;
                game.currentState = GameState::SelectionScreen;
                new_record = false;
            }
        }
                
        EndDrawing();
    }
    
    CloseWindow();

    CloseAudioDevice();

    delete resources;
    
    return 0;
}
