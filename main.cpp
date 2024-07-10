#include "battery/embed.hpp"
#include "shader.h"
#include <SDL2/SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_mutex.h>
#include <SDL_video.h>
#include <glad/glad.h>

#include <iostream>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window *window =
        SDL_CreateWindow("LearnOpenGL", 0, 0, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cout << "Failed to create SDL window" << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_GLContext gl_context;
    if (NULL == (gl_context = SDL_GL_CreateContext(window))) {
        std::cout << "Failed to create OpenGL context" << std::endl;
        SDL_Quit();
        return -1;
    } else {
        SDL_GL_MakeCurrent(window, gl_context);
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    // setup shaders
    const char *vertexShaderSource =
        b::embed<"shaders/vertex_shader.glsl">().data();

    const char *fragmentShaderSource =
        b::embed<"shaders/fragment_shader.glsl">().data();

    Shader shader_program = Shader(SourceType::SOURCE_CODE, vertexShaderSource,
                                   fragmentShaderSource);

    // setup triangle VAO and VBO
    unsigned int VAO[2];
    glGenVertexArrays(2, VAO);

    float vertices1[] = {0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
                         0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                         0.25f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
    float vertices2[] = {-0.5f, 0.0f,  0.0f,  -0.75f, -0.5f,
                         0.0f,  -0.25, -0.5f, 0.0f};
    unsigned int indices[] = {0, 1, 3, 1, 2, 3};

    unsigned int VBO[2];
    glGenBuffers(2, VBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                event.type == SDL_WINDOWEVENT &&
                    event.window.event == SDL_WINDOWEVENT_CLOSE &&
                    event.window.windowID == SDL_GetWindowID(window)) {
                done = true;
            }
        }
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.use();
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
    }
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
