#include <O3GL/Context.h>

#include <SDL3/SDL.h>

#include <stack>
#include <functional>
#include <iostream>
#include <sstream>

using namespace O3GL;

inline void SDLErrorBox(const char* title)
{
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title, SDL_GetError(), SDL_GL_GetCurrentWindow());
}

class SDLGLContext : public Context
{
private:
    SDL_Window* mWindow;
    SDL_GLContext mContext;
public:
    SDLGLContext(SDL_Window* window):
        mWindow(window)
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        mContext = SDL_GL_CreateContext(window);
        if (!mContext) {
            SDLErrorBox("GL Context Creation Failed");
            std::exit(1);
        }
    }

    ~SDLGLContext()
    {
        if (mContext)
            SDL_GL_DeleteContext(mContext);
    }

    bool MakeCurrent() override
    {
        return !SDL_GL_MakeCurrent(mWindow, mContext);
    }

    void* Load(const char* funcName) override
    {
        void* funcPtr = reinterpret_cast<void*>(SDL_GL_GetProcAddress(funcName));
        if (!funcPtr)
            std::cout << "Failed to load OpenGL function pointer \"" << funcName << "\"!\n";
        return funcPtr;
    }
};

class Defer
{
private:
    std::stack<std::function<void()>> mStack;
public:
    Defer() = default;

    void operator<<(std::function<void()> func)
    {
        mStack.push(func);
    }

    ~Defer()
    {
        while(!mStack.empty()) {
            mStack.top()();
            mStack.pop();
        }
    }
};

int main(int argc, char** argv)
{
    Defer defer;

    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        SDLErrorBox("Failed SDL Init");
        return 1;
    }

    defer << []() { SDL_Quit(); };

    SDL_Window* window = SDL_CreateWindow(
        "O3GL Example", 
        1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDLErrorBox("Failed Window Creation");
        return 1;
    }

    defer << [window]() { SDL_DestroyWindow(window); };

    SDLGLContext glContext(window);
    int res = glContext.LoadAll();
    if (res == -1) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Failed to load OpenGL functions", "Failed to load any OpenGL function pointers!", window);
        return 1;
    } else if (res) {
        std::stringstream ss;
        ss << "Failed to load ";
        ss << res;
        ss << " OpenGL function pointers!";
        std::string msg = ss.str();
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Failed to load OpenGL Functions", msg.c_str(), window);
    }

    if (!glContext.MakeCurrent()) {
        SDLErrorBox("Failed to make GLContext current");
        return 1;
    }

    char data[1024];
    Buffer buffer = glContext.GenBuffer();
    buffer.Data(Buffer::Target::Array, 1024, data, Buffer::Usage::StaticDraw);
    std::cout << buffer.GetSize(Buffer::Target::Array) << "\n";
    buffer.Storage(200, nullptr, Buffer::StorageFlags::Dynamic);

    while(1) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT)
                return 0;
        }

        SDL_GL_SwapWindow(window);
    }

    return 0;
}