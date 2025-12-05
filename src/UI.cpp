#include "UI.h"

UI::UI() : renderer(nullptr), currentTool(PEN), drawing(false), penButton{10,10,80,40}, eraserButton{100,10,80,40} {}
void UI::setRenderer(SDL_Renderer* r) { renderer = r; }
void UI::handleEvent(const SDL_Event& event, Canvas& canvas) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mx = event.button.x, my = event.button.y;
        if (mx >= penButton.x && mx <= penButton.x + penButton.w && my >= penButton.y && my <= penButton.y + penButton.h) {
            currentTool = PEN;
        } else if (mx >= eraserButton.x && mx <= eraserButton.x + eraserButton.w && my >= eraserButton.y && my <= eraserButton.y + eraserButton.h) {
            currentTool = ERASER;
        } else {
            drawing = true;
        }
    } else if (event.type == SDL_MOUSEBUTTONUP) {
        drawing = false;
    } else if (event.type == SDL_MOUSEMOTION && drawing) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        canvas.addPoint(x, y, currentTool);
    }
}
void UI::render(const Canvas& canvas) const {
    // Draw Pen button
    if (currentTool == PEN)
        SDL_SetRenderDrawColor(renderer, 200, 255, 200, 255);
    else
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &penButton);
    // Draw Eraser button
    if (currentTool == ERASER)
        SDL_SetRenderDrawColor(renderer, 255, 200, 200, 255);
    else
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_RenderFillRect(renderer, &eraserButton);
    // Draw strokes
    canvas.render();
}
