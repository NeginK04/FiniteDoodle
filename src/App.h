#pragma once
#include <SDL.h>
#include "Canvas.h"
#include "Tool.h"
#include "UI.h"

class App {
public:
    App();
    ~App();
    int run();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Canvas canvas;
    UI ui;
    bool running;
    void handleEvents();
    void render();
};
