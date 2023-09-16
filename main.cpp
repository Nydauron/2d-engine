#include <SDL_events.h>
#include <SDL_mutex.h>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL_video.h>
#include <SDL_error.h>

#include <iostream>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    
    SDL_Window *window = SDL_CreateWindow("LearnOpenGL", 0, 0, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cout << "Failed to create SDL window" << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_GLContext gl_context;
    if(NULL == (gl_context = SDL_GL_CreateContext(window))) {
        std::cout << "Failed to create OpenGL context" << std::endl;
        SDL_Quit();
        return -1;
    }
    else {
        SDL_GL_MakeCurrent(window, gl_context);
    }

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    bool done = false;
    while (!done) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                    event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window)) {
                done = true;
            }
        }
        SDL_GL_SwapWindow(window);
    }
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
