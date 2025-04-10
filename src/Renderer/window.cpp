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

    glfwSetWindowUserPointer(instance, this);
    glfwSetFramebufferSizeCallback(instance, framebufferSizeCallback);
}

Window::~Window() {
    glfwDestroyWindow(instance);
    glfwTerminate();
}

void Window::framebufferSizeCallback(GLFWwindow* window, int w, int h) {
    
    Window* windowPtr = static_cast<Window*>(glfwGetWindowUserPointer(window));

    windowPtr->width = w;
    windowPtr->height = h;
    glViewport(0, 0, w, h);
}