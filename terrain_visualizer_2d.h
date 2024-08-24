#ifndef TERRAIN_VISUALIZER_2D_H
#define TERRAIN_VISUALIZER_2D_H

#include <SDL2/SDL.h>
#include "terrain.h"
#include <functional>

class TerrainVisualizer2D {
public:
    TerrainVisualizer2D(int windowWidth, int windowHeight);
    ~TerrainVisualizer2D();

    void visualize(const Terrain& terrain);
    void animateErosion(Terrain& terrain, std::function<void(Terrain&)> erodeStep, int totalSteps, int fps);

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    int m_windowWidth;
    int m_windowHeight;

    void initSDL();
    void quitSDL();
    SDL_Color getColorForHeight(float height);
    SDL_Color lerpColor(const SDL_Color& a, const SDL_Color& b, float t);
    void drawTerrain(const Terrain& terrain);
};

#endif // TERRAIN_VISUALIZER_2D_H
