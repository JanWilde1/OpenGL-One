#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer/Window.h"

class Application
{
public:
    Application();

    void init();
    void update();
    void render();
    void cleanup();

private:
    Window* window;
};