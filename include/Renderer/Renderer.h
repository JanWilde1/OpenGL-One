#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

struct Window;

struct RenderObject
{
    glm::vec3 position;
    glm::vec3 colour;
    std::string identifier;

    RenderObject(glm::vec3 pos, glm::vec3 col, std::string id) 
        : position(pos), colour(col), identifier(id) {}
};

class Renderer
{
public:
    Renderer(Window& window);
    ~Renderer();

    bool init();
    void shutdown();

    void renderFrame(std::vector<RenderObject>& objects);
    void setClearColor(float r, float g, float b, float a);


private:
    GLuint loadShaders(const char* vertex_file_path, const char* fragment_file_path);

    bool checkShaderCompileErrors(GLuint shader, const std::string& type);
    bool checkProgramLinkErrors(GLuint program);

    Window& m_window;
    GLFWwindow* m_glfwWindowHandle; //Sort

    GLuint m_shaderProgramID;
    GLuint m_vertexArrayID;
    GLuint m_vertexBufferID;

    GLuint m_textureID;

    glm::vec4 m_clearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
};