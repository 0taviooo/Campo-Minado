#ifndef UTILS_H

#define UTILS_H

struct Utils {
    static bool timer(float currentTime, float& lastTime, float interval);
    static float center(const float f1, const float f2);
    static float centralize(float length, Vector2 posLimX);
};

float Utils::center(const float f1, const float f2) {
    return (f1 - f2)/2.f;
}

bool Utils::timer(float currentTime, float& lastTime, float interval) {
    if (currentTime - lastTime > interval) {
        lastTime = currentTime;
        return true;
    }
    return false;
}

float Utils::centralize(float length, Vector2 posLimX) {
    float gap = (posLimX.y - posLimX.x - length) / 2;
    float x = posLimX.x + gap;
    return x;
}

#endif
