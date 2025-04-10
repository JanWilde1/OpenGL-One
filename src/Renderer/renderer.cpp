#include "Renderer/Renderer.h"
#include "Renderer/Window.h"

#include <iostream>
#include <fstream>
#include <unistd.h>

Renderer::Renderer(Window& window)
    : m_window(window), m_glfwWindowHandle(window.instance)
{
    std::cout << "Renderer constructor called" << std::endl;
    init();
}

Renderer::~Renderer()
{
    std::cout << "Renderer destructor called" << std::endl;
    shutdown();
}

bool Renderer::init()
{
    m_shaderProgramID = loadShaders("../shaders/simple.vert", "../shaders/simple.frag");

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    glGenBuffers(1, &m_vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return true;
}

void Renderer::shutdown()
{
    if (m_vertexArrayID) {
        glDeleteVertexArrays(1, &m_vertexArrayID);
        m_vertexArrayID = 0;
    }
    
    if (m_vertexBufferID) {
        glDeleteBuffers(1, &m_vertexBufferID);
        m_vertexBufferID = 0;
    }
    
    if (m_shaderProgramID) {
        glDeleteProgram(m_shaderProgramID);
        m_shaderProgramID = 0;
    }
}

void Renderer::renderFrame(std::vector<RenderObject>& objects)
{
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(m_shaderProgramID);
    glBindVertexArray(m_vertexArrayID);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    for (const auto& object : objects)
    {
        std::cout << "Rendering object: " << object.identifier << std::endl;
    }

    glBindVertexArray(0);
    glUseProgram(0);

    glfwSwapBuffers(m_glfwWindowHandle);
}

void Renderer::setClearColor(float r, float g, float b, float a)
{
    m_clearColor = glm::vec4(r, g, b, a);
}

GLuint Renderer::loadShaders(const char* vertexFilePath, const char* fragmentFilePath)
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexShaderCode;

    std::ifstream checkVertexFile(vertexFilePath);
    if (!checkVertexFile.good())
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            std::cerr << "No VS found; Current working directory:" << cwd << std::endl;
        }
        
        return 0;
    }

    std::ifstream vertexShaderStream(vertexFilePath, std::ios::in);
    if (vertexShaderStream.is_open())
    {
        std::string line = "";
        while (getline(vertexShaderStream, line))
            vertexShaderCode += "\n" + line;
        vertexShaderStream.close();
    }

    std::string fragmentShaderCode;

    std::ifstream checkFragmentFile(fragmentFilePath);
    if (!checkFragmentFile.good())
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            std::cerr << "No FS found; Current working directory:" << cwd << std::endl;
        }
        
        return 0;
    }
    std::ifstream fragmentShaderStream(fragmentFilePath, std::ios::in);
    if (fragmentShaderStream.is_open())
    {
        std::string line = "";
        while (getline(fragmentShaderStream, line))
            fragmentShaderCode += "\n" + line;
        fragmentShaderStream.close();
    }

    // Compilations

    char const* vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
    glCompileShader(vertexShaderID);
    if (!checkShaderCompileErrors(vertexShaderID, vertexFilePath))
    {
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        return 0;
    }

    char const* fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
    glCompileShader(fragmentShaderID);
    if (!checkShaderCompileErrors(fragmentShaderID, fragmentFilePath))
    {
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        return 0;
    }

    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    if (!checkProgramLinkErrors(programID))
    {
        glDetachShader(programID, vertexShaderID);
        glDetachShader(programID, fragmentShaderID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);
        glDeleteProgram(programID);
        return 0;
    }

    return programID;
}

bool Renderer::checkShaderCompileErrors(GLuint shader, const std::string& type)
{
    GLint success;
    GLchar infoLog[512];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    return success;
}

bool Renderer::checkProgramLinkErrors(GLuint program)
{
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }
    return success;
}