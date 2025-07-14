// clang-format off
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// clang-format on

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Model.h"
#include "ModelManager.h"

// Vertex shader source
const char* vertexShaderSource = R"(
#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vertexColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = 3.0;  // Make points visible
    vertexColor = aColor;
}
)";

// Fragment shader source
const char* fragmentShaderSource = R"(
#version 460 core
in vec3 vertexColor;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertexColor, 1.0);
}
)";

// Camera controls
float cameraDistance = 3.0f;
float cameraAngleX = 0.0f;   // Horizontal rotation
float cameraAngleY = 30.0f;  // Vertical rotation (start at slight angle)
bool mousePressed = false;
double lastMouseX = 0.0;
double lastMouseY = 0.0;

void mouse_button_callback(GLFWwindow* window, int button, int action,
                           [[maybe_unused]] int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        } else if (action == GLFW_RELEASE) {
            mousePressed = false;
        }
    }
}

void cursor_position_callback([[maybe_unused]] GLFWwindow* window, double xpos,
                              double ypos) {
    if (mousePressed) {
        float deltaX = xpos - lastMouseX;
        float deltaY = ypos - lastMouseY;

        cameraAngleX += deltaX * 0.5f;
        cameraAngleY -= deltaY * 0.5f;

        // Clamp vertical rotation to avoid flipping
        if (cameraAngleY > 89.0f) cameraAngleY = 89.0f;
        if (cameraAngleY < -89.0f) cameraAngleY = -89.0f;

        lastMouseX = xpos;
        lastMouseY = ypos;
    }
}

void scroll_callback([[maybe_unused]] GLFWwindow* window,
                     [[maybe_unused]] double xoffset, double yoffset) {
    cameraDistance -= yoffset * 0.5f;
    if (cameraDistance < 1.0f) cameraDistance = 1.0f;
    if (cameraDistance > 20.0f) cameraDistance = 20.0f;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW for OpenGL 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window =
        glfwCreateWindow(800, 600, "3D Bunny Viewer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Set callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Enable depth testing and point size
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Compile vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    // Check vertex shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed:\n"
                  << infoLog << std::endl;
    }

    // Compile fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // Check fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed:\n"
                  << infoLog << std::endl;
    }

    // Create shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check program linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Load the bunny model using new API
    ModelManager modelManager;
    std::unique_ptr<Model> bunny;

    try {
        bunny = modelManager.loadModel("models/bunny.obj");
    } catch (const std::exception& e) {
        std::cerr << "Failed to load bunny model: " << e.what() << std::endl;
        return -1;
    }

    if (!bunny || bunny->vertices.empty()) {
        std::cerr << "Failed to load bunny model!" << std::endl;
        return -1;
    }

    // Create combined vertex data (position + color)
    std::vector<float> vertices;
    vertices.reserve(bunny->vertices.size() * 6);

    for (size_t i = 0; i < bunny->vertices.size(); ++i) {
        vertices.push_back(bunny->vertices[i].x);
        vertices.push_back(bunny->vertices[i].y);
        vertices.push_back(bunny->vertices[i].z);
        vertices.push_back(bunny->colors[i].x);
        vertices.push_back(bunny->colors[i].y);
        vertices.push_back(bunny->colors[i].z);
    }

    // Create vertex array object and buffers
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        // Clear the screen
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);

        // Model matrix - keep the bunny stationary at origin
        glm::mat4 model = glm::mat4(1.0f);

        // View matrix - orbit camera around the bunny
        float camX = sin(glm::radians(cameraAngleX)) *
                     cos(glm::radians(cameraAngleY)) * cameraDistance;
        float camY = sin(glm::radians(cameraAngleY)) * cameraDistance;
        float camZ = cos(glm::radians(cameraAngleX)) *
                     cos(glm::radians(cameraAngleY)) * cameraDistance;

        glm::vec3 cameraPos = glm::vec3(camX, camY, camZ);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

        // Projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                800.0f / 600.0f, 0.1f, 100.0f);

        // Set uniforms
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        unsigned int projLoc =
            glGetUniformLocation(shaderProgram, "projection");

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the bunny as points
        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, bunny->vertices.size());

        // Auto-rotate camera if not being controlled by mouse
        if (!mousePressed) {
            cameraAngleX += 0.5f;  // Slow rotation
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}
