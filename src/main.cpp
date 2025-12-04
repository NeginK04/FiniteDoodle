// g++ -o FiniteDoodle src/main.cpp -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2
// ./FiniteDoodle

#include <SDL.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum Tool { PEN, ERASER };
Tool currentTool = PEN;
const SDL_Rect penButton = {10, 10, 80, 40};
const SDL_Rect eraserButton = {100, 10, 80, 40};
const int ERASER_RADIUS = 12; // Eraser size

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("FiniteDoodle",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Main loop

    bool running = true;
    SDL_Event event;
    bool drawing = false;
    // Store multiple strokes, each stroke is a vector of points
    std::vector<std::vector<std::pair<int, int>>> strokes;
    std::vector<std::pair<int, int>> currentStroke;

    while (running) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(renderer);

        // Draw Pen button
        if (currentTool == PEN)
            SDL_SetRenderDrawColor(renderer, 200, 255, 200, 255); // Highlighted
        else
            SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255); // Normal
        SDL_RenderFillRect(renderer, &penButton);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // Optionally draw "Pen" text here (requires SDL_ttf for real text)

        // Draw Eraser button
        if (currentTool == ERASER)
            SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255); // Highlighted
        else
            SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255); // Normal
        SDL_RenderFillRect(renderer, &eraserButton);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        // Optionally draw "Eraser" text here

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x, my = event.button.y;
                // Check if clicked on Pen button
                if (mx >= penButton.x && mx <= penButton.x + penButton.w &&
                    my >= penButton.y && my <= penButton.y + penButton.h) {
                    currentTool = PEN;
                }
                // Check if clicked on Eraser button
                else if (mx >= eraserButton.x && mx <= eraserButton.x + eraserButton.w &&
                         my >= eraserButton.y && my <= eraserButton.y + eraserButton.h) {
                    currentTool = ERASER;
                } else {
                    drawing = true;
                    currentStroke.clear();
                }
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                drawing = false;
                if (currentTool == PEN && !currentStroke.empty()) {
                    strokes.push_back(currentStroke);
                }
            } else if (event.type == SDL_MOUSEMOTION) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (drawing && currentTool == PEN) {
                    currentStroke.push_back({x, y});
                } else if (drawing && currentTool == ERASER) {
                    // Erase points within eraser radius only when mouse is pressed
                    for (auto& stroke : strokes) {
                        stroke.erase(
                            std::remove_if(stroke.begin(), stroke.end(),
                                [x, y](const std::pair<int, int>& pt) {
                                    int dx = pt.first - x;
                                    int dy = pt.second - y;
                                    return dx*dx + dy*dy <= ERASER_RADIUS*ERASER_RADIUS;
                                }),
                            stroke.end()
                        );
                    }
                }
            }
        }

        // Draw all strokes
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Pen set to black
        for (const auto& stroke : strokes) {
            for (size_t i = 1; i < stroke.size(); ++i) {
                SDL_RenderDrawLine(renderer, stroke[i-1].first, stroke[i-1].second, stroke[i].first, stroke[i].second);
            }
        }
        // Draw the current stroke in progress (only for pen)
        if (currentTool == PEN) {
            for (size_t i = 1; i < currentStroke.size(); ++i) {
                SDL_RenderDrawLine(renderer, currentStroke[i-1].first, currentStroke[i-1].second, currentStroke[i].first, currentStroke[i].second);
            }
        }

        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}