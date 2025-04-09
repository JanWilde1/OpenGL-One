#pragma once

#include <GLFW/glfw3.h>
#include <iostream>

struct Window {
    GLFWwindow* instance;
    int width;
    int height;
    const char* title;
    float clearColor[4];

    Window(int w, int h, const char* t);

    ~Window();

    void setClearColor(float r, float g, float b, float a);

    void clear();
};