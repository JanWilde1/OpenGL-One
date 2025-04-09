#include <GLFW/glfw3.h>

#include <Renderer/Window.h>

Window::Window(int w, int h, const char* t) : width(w), height(h), title(t) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    instance = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!instance) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(instance);
    glViewport(0, 0, width, height);
}

Window::~Window() {
    glfwDestroyWindow(instance);
    glfwTerminate();
}

void Window::setClearColor(float r, float g, float b, float a) {
    clearColor[0] = r;
    clearColor[1] = g;
    clearColor[2] = b;
    clearColor[3] = a;
}

void Window::clear() {
    glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}