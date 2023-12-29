#pragma once

#include <O3GL/Types.h>
#include <O3GL/Buffer.h>

#include <memory>

#ifndef GLAPIENTRY
#if defined(APIENTRY)
#define GLAPIENTRY APIENTRY
#elif defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define GLAPIENTRY __stdcall
#else
#define GLAPIENTRY
#endif
#endif

#ifndef O3GL_CHECK_GLFUNC
#ifndef NDEBUG
#define O3GL_CHECK_GLFUNC 1
#else
#define O3GL_CHECK_GLFUNC 0
#endif
#endif

namespace O3GL {

class InternalContext;

class Context
{
public:
    InternalContext* mInternalContext;

    Buffer CreateBuffer();
    Buffer GenBuffer();
    Buffer GetBoundBuffer(Buffer::Binding binding) const;
    Buffer GetBoundBuffer(Buffer::Target target) const;

    Context();

    virtual ~Context();

    virtual void UnloadedGLFunctionCalled(const char* funcName) const;
    virtual void PreGLCall(const char* name) const;
    virtual void PostGLCall(const char* name) const;
    virtual bool MakeCurrent() = 0;
    virtual void* Load(const char* funcName) = 0;

    int LoadAll();

    #include "../src/ContextGLFuncProtos.h"
};

} // namespace O3GL