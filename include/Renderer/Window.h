#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

class Renderer;

struct Window {
    GLFWwindow* instance;

    int width;
    int height;
    const char* title;
    float clearColor[4];

    Window(int w, int h, const char* t);

    ~Window();

    static void framebufferSizeCallback(GLFWwindow* window, int w, int h);
};