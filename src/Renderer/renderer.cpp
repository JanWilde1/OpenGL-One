#define STB_IMAGE_IMPLEMENTATION

#include "Renderer/Renderer.h"
#include "Renderer/Window.h"
#include "Vendor/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <unistd.h>

float rotation = 0;

const glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};

Renderer::Renderer()
{
    std::cout << "Renderer constructor called" << std::endl;
    initTriangle();
}

Renderer::~Renderer()
{
    std::cout << "Renderer destructor called" << std::endl;
    shutdown();
}

bool Renderer::initTriangle()
{
    m_shaderProgramID = loadShaders("../shaders/simple.vert", "../shaders/simple.frag");

    GLfloat vertices[] = {
        // Front face (positive Z)
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, 0.5f,   1.0f, 0.0f,
         0.5f,  0.5f, 0.5f,   1.0f, 1.0f,
         0.5f,  0.5f, 0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f, 0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,

        // Back face (negative Z)
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        // Right face (positive X)
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        // Left face (negative X)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        // Top face (positive Y)
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

        // Bottom face (negative Y)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f
    };

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* image = stbi_load("../textures/tex1.png", &width, &height, &channels, 0);

    if (image)
    {
        GLenum format;
        if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;
        else {
            std::cerr << "Unexpected number of channels: " << channels << std::endl;
            format = GL_RGB;
        }
        
        std::cout << "Loaded texture: " << width << "x" << height << " with " << channels << " channels" << std::endl;
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        // Store texture ID
        m_textureID = texture;
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
    }

    stbi_image_free(image);
    
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    glGenBuffers(1, &m_vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); // Coordinates
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); // Texture
    glEnableVertexAttribArray(1);
    
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
    
    if (m_textureID) {
        glDeleteTextures(1, &m_textureID);
        m_textureID = 0;
    }
}

void Renderer::renderFrame(std::vector<RenderObject>& objects)
{
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(m_shaderProgramID);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -(float)glfwGetTime()));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

    unsigned int viewShaderLocation = glGetUniformLocation(m_shaderProgramID, "view");
    unsigned int projectionShaderLocation = glGetUniformLocation(m_shaderProgramID, "projection");
    unsigned int modelShaderLocation = glGetUniformLocation(m_shaderProgramID, "model");

    glUniformMatrix4fv(viewShaderLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionShaderLocation, 1, GL_FALSE, glm::value_ptr(projection));

    // Activate and bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
    
    // Set uniform if your shader uses a sampler
    // If your fragment shader has: uniform sampler2D textureSampler;
    GLint texLocation = glGetUniformLocation(m_shaderProgramID, "textureSampler");
    if (texLocation != -1) {
        glUniform1i(texLocation, 0);
    }

    for (uint i = 0; i<=10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);

        float angleOfRotation = i * 30;

        model = glm::rotate(model, glm::radians(angleOfRotation), glm::vec3(1.0f, 0.0f, 1.0f));

        glUniformMatrix4fv(modelShaderLocation, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(m_vertexArrayID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
    glUseProgram(0);
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