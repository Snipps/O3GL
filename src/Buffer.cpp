#include <O3GL/Buffer.h>
#include <O3GL/Context.h>

namespace O3GL {

struct ScopedBufferBind
{
    Buffer prev;
    Buffer::Target target;

    ScopedBufferBind(Buffer::Target target, const Buffer& buffer):
        prev(buffer.mContext->GetBoundBuffer(target)), target(target)
    {
        buffer.Bind(target);
    }

    ~ScopedBufferBind()
    {
        prev.Bind(target);
    }
};

#define BIND ScopedBufferBind bind(target, *this);
#define TARGET static_cast<GLenum>(target)
#define GLENUM(x) static_cast<GLenum>(x)

Buffer::Binding Buffer::TargetToBinding(Target target)
{
    switch (target) {
        case Target::Array:
            return Binding::Array;
        case Target::AtomicCounter:
            return Binding::AtomicCounter;
        case Target::CopyRead:
            return Binding::CopyRead;
        case Target::CopyWrite:
            return Binding::CopyRead;
        case Target::DispatchIndirect:
            return Binding::DispatchIndirect;
        case Target::DrawIndirect:
            return Binding::DrawIndirect;
        case Target::ElementArray:
            return Binding::ElementArray;
        case Target::PixelPack:
            return Binding::PixelPack;
        case Target::PixelUnpack:
            return Binding::PixelUnpack;
        case Target::Query:
            return Binding::Query;
        case Target::ShaderStorage:
            return Binding::ShaderStorage;
        case Target::Texture:
            return Binding::Texture;
        case Target::TransformFeedback:
            return Binding::TransformFeedback;
        case Target::Uniform:
            return Binding::Uniform;
        default:
            return static_cast<Binding>(0);
    };
}

Buffer::Buffer(Buffer&& other)
{
    mId = other.mId;
    mContext = other.mContext;
    mOwn = other.mOwn;
    other.mId = 0;
    other.mContext = nullptr;
    other.mOwn = false;
}

Buffer& Buffer::operator=(Buffer&& other)
{
    if (mOwn)
        Delete();
    mId = other.mId;
    mContext = other.mContext;
    mOwn = other.mOwn;
    other.mId = 0;
    other.mContext = nullptr;
    other.mOwn = false;
    return *this;
}

Buffer::~Buffer()
{
    if (mOwn)
        Delete();
}

void Buffer::Delete()
{
    if (IsBuffer())
        mContext->glDeleteBuffers(1, &mId);
}

void Buffer::Bind(Target target) const
{
    mContext->glBindBuffer(TARGET, mId);
}

void Buffer::BindBase(Target target, GLuint index) const
{
    mContext->glBindBufferBase(TARGET, index, mId);
}

void Buffer::BindRange(Target target, GLuint index, GLintptr offset, GLsizeiptr size) const
{
    mContext->glBindBufferRange(TARGET, index, mId, offset, size);
}

void Buffer::Data(Target target, GLsizeiptr size, const GLvoid* data, Usage usage)
{
    BIND
    mContext->glBufferData(TARGET, size, data, GLENUM(usage));
}

void Buffer::Data(GLsizeiptr size, const void* data, Usage usage)
{
    mContext->glNamedBufferData(mId, size, data, GLENUM(usage));
}

void Buffer::SubData(Target target, GLintptr offset, GLsizeiptr size, const GLvoid* data)
{
    BIND
    mContext->glBufferSubData(TARGET, offset, size, data);
}

void Buffer::SubData(GLintptr offset, GLsizei size, const void* data)
{
    mContext->glNamedBufferSubData(mId, offset, size, data);
}

void Buffer::Storage(Target target, GLsizeiptr size, const GLvoid* data, Bitfield<StorageFlags> flags)
{
    BIND
    mContext->glBufferStorage(TARGET, size, data, flags);
}

void Buffer::Storage(GLsizeiptr size, const void* data, Bitfield<StorageFlags> flags)
{
    mContext->glNamedBufferStorage(mId, size, data, flags);
}

void Buffer::Clear(Target target, SizedInternalFormat internalFormat, Format format, Type type, const void* data)
{
    BIND
    mContext->glClearBufferData(TARGET, GLENUM(internalFormat), GLENUM(format), GLENUM(type), data);
}

void Buffer::Clear(SizedInternalFormat internalFormat, Format format, Type type, const void* data)
{
    mContext->glClearNamedBufferData(mId, GLENUM(internalFormat), GLENUM(format), GLENUM(type), data);
}

void Buffer::ClearSub(Target target, SizedInternalFormat internalFormat, GLintptr offset, GLsizeiptr size, Format format, Type type, const void* data)
{
    BIND
    mContext->glClearBufferSubData(TARGET, GLENUM(internalFormat), offset, size, GLENUM(format), GLENUM(type), data);
}

void Buffer::ClearSub(SizedInternalFormat internalFormat, GLintptr offset, GLsizei size, Format format, Type type, const void* data)
{
    mContext->glClearNamedBufferSubData(mId, GLENUM(internalFormat), offset, size, GLENUM(format), GLENUM(type), data);
}

void Buffer::CopySubBind(const Buffer& src, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
    ScopedBufferBind writeBind(Target::CopyWrite, *this);
    ScopedBufferBind readBind(Target::CopyRead, src);
    mContext->glCopyBufferSubData(GLENUM(Target::CopyRead), GLENUM(Target::CopyWrite), readOffset, writeOffset, size);
}

void Buffer::CopySub(const Buffer& src, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)
{
    mContext->glCopyNamedBufferSubData(src.mId, mId, readOffset, writeOffset, size);
}

void* Buffer::Map(Target target, MapAccess access)
{
    BIND
    return mContext->glMapBuffer(TARGET, GLENUM(access));
}

void* Buffer::Map(MapAccess access)
{
    return mContext->glMapNamedBuffer(mId, GLENUM(access));
}

void* Buffer::MapRange(Target target, GLintptr offset, GLsizeiptr length, Bitfield<MapAccessFlags> access)
{
    BIND
    return mContext->glMapBufferRange(TARGET, offset, length, access);
}

void* Buffer::MapRange(GLintptr offset, GLsizei length, Bitfield<MapAccessFlags> access)
{
    return mContext->glMapNamedBufferRange(mId, offset, length, access);
}

void Buffer::Flush(Target target, GLintptr offset, GLsizeiptr length)
{
    BIND
    mContext->glFlushMappedBufferRange(TARGET, offset, length);
}

void Buffer::Flush(GLintptr offset, GLsizei length)
{
    mContext->glFlushMappedNamedBufferRange(mId, offset, length);
}

void Buffer::GetSubData(Target target, GLintptr offset, GLsizeiptr size, void* data) const
{
    BIND
    mContext->glGetBufferSubData(TARGET, offset, size, data);
}

void Buffer::GetSubData(GLintptr offset, GLsizei size, void* data) const
{
    mContext->glGetNamedBufferSubData(mId, offset, size, data);
}

void Buffer::Invalidate()
{
    mContext->glInvalidateBufferData(mId);
}

void Buffer::InvalidateSub(GLintptr offset, GLsizeiptr length)
{
    mContext->glInvalidateBufferSubData(mId, offset, length);
}

void Buffer::GetParam(Target target, Param param, GLint* data) const
{
    BIND
    mContext->glGetBufferParameteriv(TARGET, GLENUM(param), data);
}

void Buffer::GetParam(Target target, Param param, GLint64* data) const
{
    BIND
    mContext->glGetBufferParameteri64v(TARGET, GLENUM(param), data);
}

void Buffer::GetParam(Param param, GLint* data) const
{
    mContext->glGetNamedBufferParameteriv(mId, GLENUM(param), data);
}

void Buffer::GetParam(Param param, GLint64* data) const
{
    mContext->glGetNamedBufferParameteri64v(mId, GLENUM(param), data);
}

GLint Buffer::GetParamI(Target target, Param param) const
{
    GLint value = 0;
    GetParam(target, param, &value);
    return value;
}

GLint64 Buffer::GetParamI64(Target target, Param param) const
{
    GLint64 value = 0;
    GetParam(target, param, &value);
    return value;
}

GLint Buffer::GetParamI(Param param) const
{
    GLint value = 0;
    GetParam(param, &value);
    return value;
}

GLint64 Buffer::GetParamI64(Param param) const
{
    GLint64 value = 0;
    GetParam(param, &value);
    return value;
}

void* Buffer::GetPointer(Target target, PtrParam param) const
{
    void* ptr = nullptr;
    BIND
    mContext->glGetBufferPointerv(TARGET, GLENUM(param), &ptr);
    return ptr;
}

void* Buffer::GetPointer(PtrParam param) const
{
    void* ptr = nullptr;
    mContext->glGetNamedBufferPointerv(mId, GLENUM(param), &ptr);
    return ptr;
}

bool Buffer::IsBuffer() const
{
    return mContext->glIsBuffer(mId);
}

} // namespace O3GL