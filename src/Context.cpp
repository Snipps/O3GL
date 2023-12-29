#include <O3GL/Context.h>

#include <iostream>

namespace O3GL {

#include "InternalContext.h"

Context::Context()
{
    mInternalContext = new InternalContext();
    mInternalContext->context = this;
}

Context::~Context()
{
    delete mInternalContext;
}

Buffer Context::CreateBuffer()
{
    GLuint id = 0;
    glCreateBuffers(1, &id);
    return Buffer(id, this, true);
}

Buffer Context::GenBuffer()
{
    GLuint id = 0;
    glGenBuffers(1, &id);
    return Buffer(id, this, true);
}

Buffer Context::GetBoundBuffer(Buffer::Binding binding) const
{
    GLint bound = 0;
    glGetIntegerv(static_cast<GLenum>(binding), &bound);
    return Buffer(bound, this, false);
}

Buffer Context::GetBoundBuffer(Buffer::Target target) const
{
    return GetBoundBuffer(Buffer::TargetToBinding(target));
}

void Context::UnloadedGLFunctionCalled(const char* funcName) const
{
    std::cerr << "Unloaded OpenGL Function \"" << funcName << "\" Called!\n";
}

void Context::PreGLCall(const char* name) const {}
void Context::PostGLCall(const char* name) const {}

int Context::LoadAll()
{
    return mInternalContext->LoadAll();
}

} // namespace O3GL