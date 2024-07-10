#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "shader.h"

#include <exception>
#include <sstream>

Shader::Shader(SourceType type, const char* vertexSrc, const char* fragmentSrc)
{
    switch (type) { 
    case SOURCE_CODE: {
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSrc, NULL);
        glCompileShader(vertexShader);

        int success;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if (!success) {
            throw ShaderCompilationException(vertexShader, "VERTEX");
        }

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if (!success) {
            throw ShaderCompilationException(fragmentShader, "FRAGMENT");
        }

        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);

        if (!success) {
            throw ShaderCompilationException(shaderProgram, "LINKING");
        }

        ID = shaderProgram;

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        break;
    }
    case FILE_PATH:
        throw std::invalid_argument("Path is not supported yet");
        break;
    }
}

// use/activate the shader
void Shader::use()
{
    glUseProgram(ID);
}

// utility uniform functions
void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
