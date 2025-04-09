#include "Application/Application.h"

#include <iostream>

Application::Application()
{
    init();

    if (window) { 
        window->setClearColor(0.2f, 0.3f, 0.0f, 1.0f);

        while (!glfwWindowShouldClose(window->instance))
        {
            update();
            render();

            glfwSwapBuffers(window->instance);
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
    window->clear();
}

void Application::cleanup()
{
    delete window;
    window = nullptr;

    glfwTerminate();
}