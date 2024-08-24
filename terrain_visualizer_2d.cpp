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

SDL_Color TerrainVisualizer2D::getColorForHeight(float height, float erosionStage) {
    const SDL_Color grass = {34, 139, 34, 255};  // Forest green
    const SDL_Color dirt = {139, 69, 19, 255};   // Saddle brown
    const SDL_Color rock = {128, 128, 128, 255}; // Gray

    // Adjust these thresholds to control the distribution of terrain types
    float grassThreshold = 0.7f - erosionStage * 0.3f; // Grass decreases as erosion progresses
    float dirtThreshold = 0.3f + erosionStage * 0.4f;  // Dirt increases, then decreases

    if (height > grassThreshold) {
        return grass;
    } else if (height > dirtThreshold) {
        float t = (height - dirtThreshold) / (grassThreshold - dirtThreshold);
        return lerpColor(dirt, grass, t);
    } else {
        float t = height / dirtThreshold;
        if (erosionStage > 0.7f) {
            // In late stages, transition from dirt to rock
            float rockT = (erosionStage - 0.7f) / 0.3f;
            return lerpColor(lerpColor(rock, dirt, t), dirt, 1 - rockT);
        } else {
            return lerpColor(rock, dirt, t);
        }
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

void TerrainVisualizer2D::animateErosion(Terrain& terrain, std::function<void(Terrain&)> erodeStep, int totalSteps, int fps) {
        int delayMs = 1000 / fps;

    for (int step = 0; step < totalSteps; ++step) {
        auto start = std::chrono::high_resolution_clock::now();

        // Perform erosion step
        erodeStep(terrain);

        // Draw the terrain
        float erosionStage = static_cast<float>(step) / totalSteps;
        drawTerrain(terrain, erosionStage);

        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return;
            }
        }

        // Calculate and apply delay
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        int remainingDelay = delayMs - duration.count();
        if (remainingDelay > 0) {
            SDL_Delay(remainingDelay);
        }
    }

    // Keep the final frame visible until the window is closed
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

void TerrainVisualizer2D::drawTerrain(const Terrain& terrain, float erosionStage) {
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    int terrainWidth = terrain.getWidth();
    int terrainHeight = terrain.getHeight();

    float scaleX = static_cast<float>(m_windowWidth) / terrainWidth;
    float scaleY = static_cast<float>(m_windowHeight) / terrainHeight;

    for (int y = 0; y < terrainHeight; ++y) {
        for (int x = 0; x < terrainWidth; ++x) {
            float height = terrain.getHeight(x, y);
            SDL_Color color = getColorForHeight(height, erosionStage);
            
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
}
