#ifndef CONSTANTS_H

#define CONSTANTS_H

#include <vector>

struct GameConstants {
    static constexpr float blinkIterval = 1.f;
    static const int windowX = 708;
    static const int windowY = 640;
    static const std::vector<float> windowXPieces;
    static const std::vector<float> windowYPieces;
    
    static std::vector<float> generatePieces() {
        std::vector<float> v = {};
        int num = 12;
        for (int i = 0; i < num; ++i) {
            v.push_back(windowX/num*i);
        }
        return v;
    }

};

const std::vector<float> GameConstants::windowXPieces = GameConstants::generatePieces();
const std::vector<float> GameConstants::windowYPieces = GameConstants::generatePieces();

struct FontSize {
    static constexpr float h1 = 60.f;
    static constexpr float h2 = 30.f;
    static constexpr float h3 = 25.f;
    static constexpr float body = 20.f;
    static constexpr float note = 15.f;
};

#endif
