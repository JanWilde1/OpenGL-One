// NOT YET IMPLEMENTED

#pragma once

#include <GL/glew.h>
#include <string>

#include <glm/glm.hpp>


class Shader {
    public:

    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();

    void useShader() const;

    void setBoolean() const;
    void setInt() const;
    void setFloat() const;

    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setMat4(const std::string& name, const glm::mat4& value) const;

    private:

    GLuint m_programID;

    bool compile(const std::string& vertexSource, const std::string& fragmentSource);
    bool checkShaderCompileErrors(GLuint shader, const std::string& type);
};