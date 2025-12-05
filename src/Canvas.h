#pragma once
#include <SDL.h>
#include <vector>
#include "Tool.h"

class Canvas {
public:
    Canvas();
    void setRenderer(SDL_Renderer* r);
    void addPoint(int x, int y, Tool tool);
    void eraseAt(int x, int y);
    void render() const;
private:
    SDL_Renderer* renderer;
    std::vector<std::vector<std::pair<int, int>>> strokes;
    std::vector<std::pair<int, int>> currentStroke;
};
