// g++ -o FiniteDoodle src/main.cpp -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2
// ./FiniteDoodle

#include <SDL.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

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

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                drawing = true;
                currentStroke.clear(); // Start a new stroke
            } else if (event.type == SDL_MOUSEBUTTONUP) {
                drawing = false;
                if (!currentStroke.empty()) {
                    strokes.push_back(currentStroke); // Save the stroke
                }
            } else if (event.type == SDL_MOUSEMOTION && drawing) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                currentStroke.push_back({x, y}); // Store the point in current stroke
            }
        }

        // Draw all strokes
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Pen set to black
        for (const auto& stroke : strokes) {
            for (size_t i = 1; i < stroke.size(); ++i) {
                // Making pen stroke continuous
                SDL_RenderDrawLine(renderer, stroke[i-1].first, stroke[i-1].second, stroke[i].first, stroke[i].second);
            }
        }

        // Draw the current stroke in progress
        for (size_t i = 1; i < currentStroke.size(); ++i) {
            SDL_RenderDrawLine(renderer, currentStroke[i-1].first, currentStroke[i-1].second, currentStroke[i].first, currentStroke[i].second);
        }

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}