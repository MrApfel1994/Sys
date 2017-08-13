#include "PlatformSDL.h"

#include <SDL2/SDL.h>

#include "AssetFileIO.h"

sys::Platform::~Platform() {
    Release();
}

int sys::Platform::Init(const std::string &window_name, int w, int h) {
    Release();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    window_ = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window_) return -1;

    width_ = w;
    height_ = h;

#if defined(USE_GL_RENDER)
    gl_ctx_ = SDL_GL_CreateContext(window_);
#if !defined(__EMSCRIPTEN__)
    SDL_GL_SetSwapInterval(0);
#endif
#elif defined(USE_SW_RENDER)
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) return -1;

    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
    if (!texture_) return -1;
#endif

    sys::InitWorker();

    return 0;
}

void sys::Platform::Release() {
#if defined(USE_GL_RENDER)
    if (gl_ctx_) {
        SDL_GL_DeleteContext(gl_ctx_);
        gl_ctx_ = nullptr;
    }
#elif defined(USE_SW_RENDER)
    if (texture_) {
        SDL_DestroyTexture(texture_);
        texture_ = nullptr;
    }
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
#endif
    if (window_) {
        width_ = 0;
        height_ = 0;
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        SDL_Quit();
    }

    sys::StopWorker();
}

void sys::Platform::DrawPixels(const void *pixels) {
#if defined(USE_GL_RENDER)
#elif defined(USE_SW_RENDER)
    if (pixels) {
        SDL_UpdateTexture(texture_, NULL, pixels, width_ * sizeof(Uint32));
        SDL_RenderClear(renderer_);
        SDL_RenderCopy(renderer_, texture_, NULL, NULL);
    }
#endif
}

void sys::Platform::EndFrame() {
#if defined(USE_GL_RENDER)
    SDL_GL_SwapWindow(window_);
#elif defined(USE_SW_RENDER)
    SDL_RenderPresent(renderer_);
#endif
}