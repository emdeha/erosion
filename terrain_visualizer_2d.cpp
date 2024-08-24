#include "terrain_visualizer_2d.h"
#include <stdexcept>
#include <algorithm>

TerrainVisualizer2D::TerrainVisualizer2D(int windowWidth, int windowHeight)
    : m_window(nullptr), m_renderer(nullptr), m_windowWidth(windowWidth), m_windowHeight(windowHeight) {
    initSDL();
}

TerrainVisualizer2D::~TerrainVisualizer2D() {
    quitSDL();
}

void TerrainVisualizer2D::visualize(const Terrain& terrain) {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    int terrainWidth = terrain.getWidth();
    int terrainHeight = terrain.getHeight();

    float scaleX = static_cast<float>(m_windowWidth) / terrainWidth;
    float scaleY = static_cast<float>(m_windowHeight) / terrainHeight;

    for (int y = 0; y < terrainHeight; ++y) {
        for (int x = 0; x < terrainWidth; ++x) {
            float height = terrain.getHeight(x, y);
            SDL_Color color = getColorForHeight(height);
            
            SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
            
            SDL_Rect rect;
            rect.x = static_cast<int>(x * scaleX);
            rect.y = static_cast<int>(y * scaleY);
            rect.w = static_cast<int>(scaleX) + 1;
            rect.h = static_cast<int>(scaleY) + 1;
            
            SDL_RenderFillRect(m_renderer, &rect);
        }
    }

    SDL_RenderPresent(m_renderer);

    // Keep the window open until it's closed
    SDL_Event event;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_Delay(100);
    }
}

void TerrainVisualizer2D::initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("SDL could not initialize! SDL_Error: " + std::string(SDL_GetError()));
    }

    m_window = SDL_CreateWindow("Terrain Visualization", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_windowWidth, m_windowHeight, SDL_WINDOW_SHOWN);
    if (m_window == nullptr) {
        throw std::runtime_error("Window could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr) {
        throw std::runtime_error("Renderer could not be created! SDL_Error: " + std::string(SDL_GetError()));
    }
}

void TerrainVisualizer2D::quitSDL() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

SDL_Color TerrainVisualizer2D::getColorForHeight(float height) {
    // Define color stops for our gradient
    const SDL_Color deepWater    = {0, 0, 128, 255};     // Deep blue
    const SDL_Color shallowWater = {0, 128, 255, 255};   // Light blue
    const SDL_Color beach        = {240, 240, 64, 255};  // Sand yellow
    const SDL_Color grass        = {32, 160, 0, 255};    // Green
    const SDL_Color forest       = {0, 96, 0, 255};      // Dark green
    const SDL_Color rock         = {96, 96, 96, 255};    // Gray
    const SDL_Color snow         = {255, 255, 255, 255}; // White

    // Define the thresholds for each terrain type
    if (height < 0.2f) {
        return lerpColor(deepWater, shallowWater, height / 0.2f);
    } else if (height < 0.3f) {
        return lerpColor(shallowWater, beach, (height - 0.2f) / 0.1f);
    } else if (height < 0.5f) {
        return lerpColor(beach, grass, (height - 0.3f) / 0.2f);
    } else if (height < 0.7f) {
        return lerpColor(grass, forest, (height - 0.5f) / 0.2f);
    } else if (height < 0.9f) {
        return lerpColor(forest, rock, (height - 0.7f) / 0.2f);
    } else {
        return lerpColor(rock, snow, (height - 0.9f) / 0.1f);
    }
}

SDL_Color TerrainVisualizer2D::lerpColor(const SDL_Color& a, const SDL_Color& b, float t) {
    return SDL_Color{
        static_cast<Uint8>(a.r + t * (b.r - a.r)),
        static_cast<Uint8>(a.g + t * (b.g - a.g)),
        static_cast<Uint8>(a.b + t * (b.b - a.b)),
        255
    };
}
