#include "Canvas.h"

Canvas::Canvas() : renderer(nullptr) {}
void Canvas::setRenderer(SDL_Renderer* r) { renderer = r; }
void Canvas::addPoint(int x, int y, Tool tool) {
    if (tool == PEN) {
        currentStroke.push_back({x, y});
    } else if (tool == ERASER) {
        eraseAt(x, y);
    }
}
void Canvas::eraseAt(int x, int y) {
    const int ERASER_RADIUS = 12;
    std::vector<std::vector<std::pair<int, int>>> newStrokes;
    for (auto& stroke : strokes) {
        std::vector<std::pair<int, int>> segment;
        for (const auto& pt : stroke) {
            int dx = pt.first - x;
            int dy = pt.second - y;
            if (dx*dx + dy*dy > ERASER_RADIUS*ERASER_RADIUS) {
                segment.push_back(pt);
            } else {
                if (!segment.empty()) {
                    newStrokes.push_back(segment);
                    segment.clear();
                }
            }
        }
        if (!segment.empty()) newStrokes.push_back(segment);
    }
    strokes = newStrokes;
}
void Canvas::render() const {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (const auto& stroke : strokes) {
        for (size_t i = 1; i < stroke.size(); ++i) {
            SDL_RenderDrawLine(renderer, stroke[i-1].first, stroke[i-1].second, stroke[i].first, stroke[i].second);
        }
    }
    for (size_t i = 1; i < currentStroke.size(); ++i) {
        SDL_RenderDrawLine(renderer, currentStroke[i-1].first, currentStroke[i-1].second, currentStroke[i].first, currentStroke[i].second);
    }
}
