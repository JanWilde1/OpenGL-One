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

    window = new Window(800, 800, "OpenGL Application");

    renderer = new Renderer(*window);

    renderer->setClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    if (!window || !window->instance)
    {
        std::cerr << "Failed to Create Window" << std::endl;
        glfwTerminate();
        return;
    }
    
    glfwMakeContextCurrent(window->instance);

    GLenum err = glewInit();

    if (err != GLEW_OK)
    {
        std::cerr << "Failed to Init GLEW: " << glewGetErrorString(err) << std::endl;
        return;
    }

    glViewport(0, 0, window->width, window->height);
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

    glfwTerminate();
}