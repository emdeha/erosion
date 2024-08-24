#include "terrain_visualizer_3d.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec3 aNormal;
    out vec3 FragPos;
    out vec3 Normal;
    out vec3 Color;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = mat3(transpose(inverse(model))) * aNormal;
        Color = aColor;
        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec3 FragPos;
    in vec3 Normal;
    in vec3 Color;
    out vec4 FragColor;
    uniform vec3 lightPos;
    uniform vec3 viewPos;
    void main() {
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
        vec3 ambient = 0.3 * vec3(1.0, 1.0, 1.0);
        vec3 result = (ambient + diffuse) * Color;
        FragColor = vec4(result, 1.0);
    }
)";

TerrainVisualizer3D::TerrainVisualizer3D(int windowWidth, int windowHeight)
    : m_windowWidth(windowWidth), m_windowHeight(windowHeight) {
    initOpenGL();
    setupShaders();
    setupBuffers();
}

TerrainVisualizer3D::~TerrainVisualizer3D() {
    cleanupOpenGL();
}

void TerrainVisualizer3D::initOpenGL() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Terrain Erosion 3D", NULL, NULL);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);

    if (glewInit() != GLEW_OK) {
        throw std::runtime_error("Failed to initialize GLEW");
    }

    glEnable(GL_DEPTH_TEST);
}

void TerrainVisualizer3D::cleanupOpenGL() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteProgram(m_shaderProgram);

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void TerrainVisualizer3D::setupShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void TerrainVisualizer3D::setupBuffers() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
}

void TerrainVisualizer3D::updateBuffers(const Terrain& terrain, float erosionStage) {
    m_vertices.clear();
    m_indices.clear();

    m_terrainWidth = terrain.getWidth();
    m_terrainHeight = terrain.getHeight();

    // Set camera position at one edge of the terrain
    m_cameraPos = glm::vec3(-10.0f, m_terrainHeight / 2.0f, m_terrainHeight / 2.0f);
    m_cameraTarget = glm::vec3(m_terrainWidth / 2.0f, 0.0f, m_terrainHeight / 2.0f);

    for (int z = 0; z < m_terrainHeight; ++z) {
        for (int x = 0; x < m_terrainWidth; ++x) {
            float y = terrain.getHeight(x, z) * 20; // Amplify height for better visibility
            glm::vec3 color = getColorForHeight(terrain.getHeight(x, z), erosionStage);
            addCube(x, y, z, color);
        }
    }

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(GLfloat), m_vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void TerrainVisualizer3D::drawTerrain() {
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f); // Sky blue
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shaderProgram);

    // Create view matrix
    glm::mat4 view = glm::lookAt(m_cameraPos, m_cameraTarget, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 
        static_cast<float>(m_windowWidth) / m_windowHeight, 0.1f, 1000.0f);

    GLuint viewLoc = glGetUniformLocation(m_shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    GLuint projectionLoc = glGetUniformLocation(m_shaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glm::mat4 model = glm::mat4(1.0f);
    GLuint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glm::vec3 lightPos(m_terrainWidth / 2.0f, m_terrainHeight * 2.0f, m_terrainHeight / 2.0f);
    GLuint lightPosLoc = glGetUniformLocation(m_shaderProgram, "lightPos");
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));

    GLuint viewPosLoc = glGetUniformLocation(m_shaderProgram, "viewPos");
    glUniform3fv(viewPosLoc, 1, glm::value_ptr(m_cameraPos));

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glfwSwapBuffers(m_window);
}

glm::vec3 TerrainVisualizer3D::getColorForHeight(float height, float erosionStage) {
    const glm::vec3 water(0.0f, 0.2f, 0.8f);
    const glm::vec3 sand(0.95f, 0.87f, 0.6f);
    const glm::vec3 grass(0.13f, 0.55f, 0.13f);
    const glm::vec3 rock(0.5f, 0.5f, 0.5f);
    const glm::vec3 snow(1.0f, 1.0f, 1.0f);

    float sandThreshold = 0.1f;
    float grassThreshold = 0.3f;
    float rockThreshold = 0.7f;
    float snowThreshold = 0.9f;

    if (height < sandThreshold) {
        return glm::mix(water, sand, height / sandThreshold);
    } else if (height < grassThreshold) {
        return glm::mix(sand, grass, (height - sandThreshold) / (grassThreshold - sandThreshold));
    } else if (height < rockThreshold) {
        return glm::mix(grass, rock, (height - grassThreshold) / (rockThreshold - grassThreshold));
    } else if (height < snowThreshold) {
        return glm::mix(rock, snow, (height - rockThreshold) / (snowThreshold - rockThreshold));
    } else {
        return snow;
    }
}

void TerrainVisualizer3D::addCube(float x, float y, float z, const glm::vec3& color) {
    float vertices[] = {
        // positions          // colors           // normals
        0.0f, 0.0f, 0.0f,  color.r, color.g, color.b,  0.0f,  0.0f, -1.0f,
        1.0f, 0.0f, 0.0f,  color.r, color.g, color.b,  0.0f,  0.0f, -1.0f,
        1.0f, 1.0f, 0.0f,  color.r, color.g, color.b,  0.0f,  0.0f, -1.0f,
        0.0f, 1.0f, 0.0f,  color.r, color.g, color.b,  0.0f,  0.0f, -1.0f,
        
        0.0f, 0.0f, 1.0f,  color.r, color.g, color.b,  0.0f,  0.0f,  1.0f,
        1.0f, 0.0f, 1.0f,  color.r, color.g, color.b,  0.0f,  0.0f,  1.0f,
        1.0f, 1.0f, 1.0f,  color.r, color.g, color.b,  0.0f,  0.0f,  1.0f,
        0.0f, 1.0f, 1.0f,  color.r, color.g, color.b,  0.0f,  0.0f,  1.0f,
        
        0.0f, 1.0f, 1.0f,  color.r, color.g, color.b,  -1.0f,  0.0f,  0.0f,
        0.0f, 1.0f, 0.0f,  color.r, color.g, color.b,  -1.0f,  0.0f,  0.0f,
        0.0f, 0.0f, 0.0f,  color.r, color.g, color.b,  -1.0f,  0.0f,  0.0f,
        0.0f, 0.0f, 1.0f,  color.r, color.g, color.b,  -1.0f,  0.0f,  0.0f,
        
        1.0f, 1.0f, 1.0f,  color.r, color.g, color.b,  1.0f,  0.0f,  0.0f,
        1.0f, 1.0f, 0.0f,  color.r, color.g, color.b,  1.0f,  0.0f,  0.0f,
        1.0f, 0.0f, 0.0f,  color.r, color.g, color.b,  1.0f,  0.0f,  0.0f,
        1.0f, 0.0f, 1.0f,  color.r, color.g, color.b,  1.0f,  0.0f,  0.0f,
        
        0.0f, 0.0f, 0.0f,  color.r, color.g, color.b,  0.0f, -1.0f,  0.0f,
        1.0f, 0.0f, 0.0f,  color.r, color.g, color.b,  0.0f, -1.0f,  0.0f,
        1.0f, 0.0f, 1.0f,  color.r, color.g, color.b,  0.0f, -1.0f,  0.0f,
        0.0f, 0.0f, 1.0f,  color.r, color.g, color.b,  0.0f, -1.0f,  0.0f,
        
        0.0f, 1.0f, 0.0f,  color.r, color.g, color.b,  0.0f,  1.0f,  0.0f,
        1.0f, 1.0f, 0.0f,  color.r, color.g, color.b,  0.0f,  1.0f,  0.0f,
        1.0f, 1.0f, 1.0f,  color.r, color.g, color.b,  0.0f,  1.0f,  0.0f,
        0.0f, 1.0f, 1.0f,  color.r, color.g, color.b,  0.0f,  1.0f,  0.0f
    };

    for (int i = 0; i < 24; ++i) {
        vertices[i*9] += x;
        vertices[i*9 + 1] += y;
        vertices[i*9 + 2] += z;
    }

    unsigned int offset = m_vertices.size() / 9;
    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    m_vertices.insert(m_vertices.end(), std::begin(vertices), std::end(vertices));

    for (unsigned int index : indices) {
        m_indices.push_back(offset + index);
    }
}

void TerrainVisualizer3D::visualize(const Terrain& terrain) {
    updateBuffers(terrain, 0.0f);
    
    while (!glfwWindowShouldClose(m_window)) {
        drawTerrain();
        glfwPollEvents();
    }
}

void TerrainVisualizer3D::animateErosion(Terrain& terrain, std::function<void(Terrain&)> erodeStep, int totalSteps, int fps) {
    int delayMs = 1000 / fps;

    for (int step = 0; step < totalSteps; ++step) {
        auto start = std::chrono::high_resolution_clock::now();

        erodeStep(terrain);

        float erosionStage = static_cast<float>(step) / totalSteps;
        updateBuffers(terrain, erosionStage);
        drawTerrain();

        glfwPollEvents();
        if (glfwWindowShouldClose(m_window)) {
            break;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        int remainingDelay = delayMs - duration.count();
        if (remainingDelay > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(remainingDelay));
        }
    }

    while (!glfwWindowShouldClose(m_window)) {
        drawTerrain();
        glfwPollEvents();
    }
}
