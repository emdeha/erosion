#ifndef TERRAIN_VISUALIZER_2D_H
#define TERRAIN_VISUALIZER_2D_H

#include <SDL2/SDL.h>
#include "terrain.h"

class TerrainVisualizer2D {
public:
    TerrainVisualizer2D(int windowWidth, int windowHeight);
    ~TerrainVisualizer2D();

    void visualize(const Terrain& terrain);

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    int m_windowWidth;
    int m_windowHeight;

    void initSDL();
    void quitSDL();
    SDL_Color getColorForHeight(float height);
};

#endif // TERRAIN_VISUALIZER_2D_H
