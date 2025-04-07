#ifndef RESOURCE_H

#define RESOURCE_H

#include "raylib.h"
#include <unordered_map>
#include <iostream>

using Images = std::unordered_map<std::string, Texture2D>;
using Musics = std::unordered_map<std::string, Music>;
using Sounds = std::unordered_map<std::string, Sound>;

struct Resources {
    
    Musics musics;
    Images textures;
    Sounds sounds;
    
    void loadMusics() {
        musics = {
            {"main_title_music", LoadMusicStream("sfx/main_title_music.mp3")},
            {"menu_music", LoadMusicStream("sfx/menu_music.mp3")},
            {"playing_music", LoadMusicStream("sfx/playing_music.mp3")},
            {"game_over_music", LoadMusicStream("sfx/game_over_music.mp3")}
        };
    }
    void loadSounds() {
        sounds = {
            {"explosion_sound", LoadSound("sfx/mus-explosion.mp3")},
            {"swipe_sound", LoadSound("sfx/mus-sfx-swipe.mp3")},
            {"buyitem_sound", LoadSound("sfx/snd-sfx-buyitem.mp3")},
            {"dumbvictory_sound", LoadSound("sfx/snd-sfx-dumbvictory.mp3")},
            {"escaped_sound", LoadSound("sfx/snd-escaped.mp3")},
            {"switchpull_sound", LoadSound("sfx/snd-sfx-switchpull.mp3")},
            {"select_sound", LoadSound("sfx/undertale-select-sound.mp3")},
        };
    }
    void loadTextures2D() {
        textures = {
            {"shovel_sprite", LoadTexture("gfx/shovel.png")},
            {"flag_sprite", LoadTexture("gfx/flag.png")},
        };
    }
    Resources() {
        loadMusics();
        loadSounds();
        loadTextures2D();
    }
    ~Resources() {
        for (auto& [key, music]: musics) {
            UnloadMusicStream(music);
        }
        for (auto& [key, sound]: sounds) {
            UnloadSound(sound);
        }
        for (auto& [key, texture]: textures) {
            UnloadTexture(texture);
        }
    }
};

#endif
