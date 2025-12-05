#pragma once
#include <SDL.h>
#include "Canvas.h"
#include "Tool.h"

class UI {
public:
    UI();
    void setRenderer(SDL_Renderer* r);
    void handleEvent(const SDL_Event& event, Canvas& canvas);
    void render(const Canvas& canvas) const;
private:
    SDL_Renderer* renderer;
    Tool currentTool;
    bool drawing;
    SDL_Rect penButton;
    SDL_Rect eraserButton;
};
