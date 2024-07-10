#pragma once

#include <glad/glad.h>

#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
  
enum SourceType {
    SOURCE_CODE,
    FILE_PATH,
};

class ShaderCompilationException: public std::exception {
private:
    std::string message;
public:
    ShaderCompilationException(unsigned int shader_id, std::string shader_type) : exception() {
        char info_log[512];
        glGetShaderInfoLog(shader_id, sizeof(info_log), NULL, info_log);

        std::transform(
            shader_type.begin(),
            shader_type.end(),
            shader_type.begin(),
            [](unsigned char c) { return std::toupper(c); }
        );

        std::ostringstream o;
        o <<"Error: SHADER::" << shader_type << "::COMPILATION_FAILED\n" << info_log << std::endl;

        message = o.str();
    }

    virtual const char* what() const throw() {
        return message.c_str();
    }
};

class Shader {
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader(SourceType type, const char *vertexPath, const char *fragmentPath);
    Shader(SourceType type, const char **vertexSrc, const char**fragmentSrc);

    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
};
