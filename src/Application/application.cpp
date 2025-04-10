#include "Application/Application.h"
#include "Renderer/Renderer.h"

#include <iostream>
#include <vector>

Application::Application()
{
    init();

    if (window) { 
        while (!glfwWindowShouldClose(window->instance))
        {
            update();
            render();

            glfwPollEvents();
        }
    }

    cleanup();
}

void Application::init()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to Init GLFW" << std::endl;
        return;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = new Window(800, 800, "OpenGL Application");
    
    if (!window || !window->instance)
    {
        std::cerr << "Failed to Create Window" << std::endl;
        glfwTerminate();
        return;
    }
    
    glfwMakeContextCurrent(window->instance);
    
    // Initialize GLEW here
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "Failed to Init GLEW: " << glewGetErrorString(err) << std::endl;
        return;
    }

    renderer = new Renderer(*window);
    renderer->setClearColor(0.5f, 0.1f, 0.1f, 1.0f);
}

void Application::update()
{
    
}

void Application::render()
{
    std::vector<RenderObject> objects;

    renderer->renderFrame(objects);
}

void Application::cleanup()
{
    delete window;
    window = nullptr;

    delete renderer;
    renderer = nullptr;

    glfwTerminate();
}