#ifndef TERRAIN_VISUALIZER_3D_H
#define TERRAIN_VISUALIZER_3D_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "terrain.h"
#include <functional>
#include <vector>

class TerrainVisualizer3D {
public:
    TerrainVisualizer3D(int windowWidth, int windowHeight);
    ~TerrainVisualizer3D();

    void visualize(const Terrain& terrain);
    void animateErosion(Terrain& terrain, std::function<void(Terrain&)> erodeStep, int totalSteps, int fps);

private:
    GLFWwindow* m_window;
    int m_windowWidth;
    int m_windowHeight;

    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_indices;
    GLuint m_VBO, m_VAO, m_EBO;
    GLuint m_shaderProgram;

    glm::vec3 m_cameraPos;
    glm::vec3 m_cameraTarget;
    int m_terrainWidth;
    int m_terrainHeight;

    void initOpenGL();
    void cleanupOpenGL();
    void setupShaders();
    void setupBuffers();
    void updateBuffers(const Terrain& terrain, float erosionStage);
    void drawTerrain();
    glm::vec3 getColorForHeight(float height, float erosionStage);
    void addCube(float x, float y, float z, const glm::vec3& color);
};

#endif // TERRAIN_VISUALIZER_3D_H
