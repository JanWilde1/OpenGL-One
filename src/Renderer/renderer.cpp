#include "Renderer/Renderer.h"
#include "Renderer/Window.h"

#include <iostream>

Renderer::Renderer(Window& window)
    : m_window(window), m_glfwWindowHandle(window.instance)
{
    std::cout << "Renderer constructor called" << std::endl;
}

void Renderer::renderFrame(std::vector<RenderObject>& objects)
{
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    for (const auto& object : objects)
    {
        // Render each object
        std::cout << "Rendering object: " << object.identifier << std::endl;
        // Here you would typically bind the object's VAO and draw it
    }

    glfwSwapBuffers(m_glfwWindowHandle);
}

void Renderer::setClearColor(float r, float g, float b, float a)
{
    m_clearColor = glm::vec4(r, g, b, a);
}