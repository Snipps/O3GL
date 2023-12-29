#pragma once

#include <O3GL/Types.h>
#include <O3GL/Bitfield.h>

namespace O3GL {

class Context;

class Buffer
{
public:
    enum class Target : GLenum
    {
        Array = 0x8892,
        AtomicCounter = 0x92C0,
        CopyRead = 0x8F36,
        CopyWrite = 0x8F37,
        DispatchIndirect = 0x90EE,
        DrawIndirect = 0x8F3F,
        ElementArray = 0x8893,
        PixelPack = 0x88EB,
        PixelUnpack = 0x88EC,
        Query = 0x9192,
        ShaderStorage = 0x90D2,
        Texture = 0x8C2A,
        TransformFeedback = 0x8C8E,
        Uniform = 0x8A11
    };

    enum class Binding : GLenum
    {
        Array = 0x8894,
        AtomicCounter = 0x92C1,
        CopyRead = 0x8F36,
        CopyWrite = 0x8F37,
        DispatchIndirect = 0x90EF,
        DrawIndirect = 0x8F43,
        ElementArray = 0x8895,
        PixelPack = 0x88ED,
        PixelUnpack = 0x88EF,
        Query = 0x9193,
        ShaderStorage = 0x90D3,
        Texture = 0x8C2A,
        TransformFeedback = 0x8C8F,
        Uniform = 0x8A28
    };

    enum class Usage : GLenum
    {
        StreamDraw = 0x88E0,
        StreamRead = 0x88E1,
        StreamCopy = 0x88E2,
        StaticDraw = 0x88E4,
        StaticRead = 0x88E5,
        StaticCopy = 0x88E6,
        DynamicDraw = 0x88E8,
        DynamicRead = 0x88E9,
        DynamicCopy = 0x88EA
    };

    enum class StorageFlags : GLbitfield
    {
        Dynamic = 0x0100,
        MapRead = 0x0001,
        MapWrite = 0x0002,
        MapPersistent = 0x0040,
        MapCoherent = 0x0080,
        ClientStorage = 0x0200
    };

    enum class Param : GLenum
    {
        MapAccess = 0x88BB,
        MapAccessFlags = 0x911F,
        Immutable = 0x821F,
        Mapped = 0x88BC,
        MapLength = 0x9120,
        MapOffset = 0x9121,
        Size = 0x8764,
        StorageFlags = 0x8220,
        Usage = 0x8765
    };

    enum class MapAccess : GLenum
    {
        ReadOnly = 0x88B8,
        WriteOnly = 0x88B9,
        ReadWrite = 0x88BA
    };

    enum class MapAccessFlags : GLbitfield
    {
        Read = 0x0001,
        Write = 0x0002,
        Persistent = 0x0040,
        Coherent = 0x0080,
        InvalidateRange = 0x0004,
        InvalidateBuffer = 0x0008,
        FlushExplicit = 0x0010,
        Unsynchronized = 0x0020
    };

    enum class PtrParam : GLenum
    {
        MapPointer = 0x88BD
    };
private:
    GLuint mId;
    const Context* mContext;
    bool mOwn;

    friend class Context;
    friend class ScopedBufferBind;

    inline Buffer(GLuint id, const Context* context, bool own) :
        mId(id), mContext(context), mOwn(own)
    {}
public:
    static Binding TargetToBinding(Target target);

    Buffer() = delete;
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other);
    Buffer& operator=(Buffer&& other);

    ~Buffer();

    void Delete();

    void Bind(Target target) const;
    void BindBase(Target target, GLuint index) const;
    void BindRange(Target target, GLuint index, GLintptr offset, GLsizeiptr size) const;
    
    void Data(Target target, GLsizeiptr size, const GLvoid* data, Usage usage);
    void Data(GLsizeiptr size, const void* data, Usage usage);
    void SubData(Target target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
    void SubData(GLintptr offset, GLsizei size, const void* data);

    void Storage(Target target, GLsizeiptr size, const GLvoid* data, Bitfield<StorageFlags> flags);
    void Storage(GLsizeiptr size, const void* data, Bitfield<StorageFlags> flags);

    void Clear(Target target, SizedInternalFormat internalFormat, Format format, Type type, const void* data);
    void Clear(SizedInternalFormat internalFormat, Format format, Type type, const void* data);
    void ClearSub(Target target, SizedInternalFormat internalFormat, GLintptr offset, GLsizeiptr size, Format format, Type type, const void* data);
    void ClearSub(SizedInternalFormat internalFormat, GLintptr offset, GLsizei size, Format format, Type type, const void* data);

    void CopySubBind(const Buffer& src, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
    void CopySub(const Buffer& src, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);

    void* Map(Target target, MapAccess access);
    void* Map(MapAccess access);
    void* MapRange(Target target, GLintptr offset, GLsizeiptr length, Bitfield<MapAccessFlags> access);
    void* MapRange(GLintptr offset, GLsizei length, Bitfield<MapAccessFlags> access);
    void Flush(Target target, GLintptr offset, GLsizeiptr length);
    void Flush(GLintptr offset, GLsizei length);

    void GetSubData(Target target, GLintptr offset, GLsizeiptr size, void* data) const;
    void GetSubData(GLintptr offset, GLsizei size, void* data) const;

    void Invalidate();
    void InvalidateSub(GLintptr offset, GLsizeiptr length);

    void GetParam(Target target, Param param, GLint* data) const;
    void GetParam(Target target, Param param, GLint64* data) const;
    void GetParam(Param param, GLint* data) const;
    void GetParam(Param param, GLint64* data) const;
    GLint GetParamI(Target target, Param param) const;
    GLint64 GetParamI64(Target target, Param param) const;
    GLint GetParamI(Param param) const;
    GLint64 GetParamI64(Param param) const;
    void* GetPointer(Target target, PtrParam param) const;
    void* GetPointer(PtrParam param) const;
    bool IsBuffer() const;

    inline operator bool() const { return IsBuffer(); }
    inline MapAccess GetMapAccess(Target target) const { return static_cast<MapAccess>(GetParamI(target, Param::MapAccess)); }
    inline MapAccess GetMapAccess() const { return static_cast<MapAccess>(GetParamI(Param::MapAccess)); }
    inline Bitfield<MapAccessFlags> GetMapAccessFlags(Target target) const { return GetParamI(target, Param::MapAccessFlags); }
    inline Bitfield<MapAccessFlags> GetMapAccessFlags() const { return GetParamI(Param::MapAccessFlags); }
    inline bool IsImmutableStorage(Target target) const { return GetParamI(target, Param::Immutable); }
    inline bool IsImmutableStorage() const { return GetParamI(Param::Immutable); }
    inline bool IsMapped(Target target) const { return GetParamI(target, Param::Mapped); }
    inline bool IsMapped() const { return GetParamI(Param::Mapped); }
    inline GLint64 GetMapLength(Target target) const { return GetParamI64(target, Param::MapLength); }
    inline GLint64 GetMapLength() const { return GetParamI64(Param::MapLength); }
    inline GLint64 GetMapOffset(Target target) const { return GetParamI64(target, Param::MapOffset); }
    inline GLint64 GetMapOffset() const { return GetParamI64(Param::MapOffset); }
    inline GLint64 GetSize(Target target) const { return GetParamI64(target, Param::Size); }
    inline GLint64 GetSize() const { return GetParamI64(Param::Size); }
    inline Bitfield<StorageFlags> GetStorageFlags(Target target) const { return GetParamI(target, Param::StorageFlags); }
    inline Bitfield<StorageFlags> GetStorageFlags() const { return GetParamI(Param::StorageFlags); }
    inline Usage GetUsage(Target target) const { return static_cast<Usage>(GetParamI(target, Param::Usage)); }
    inline Usage GetUsage() const { return static_cast<Usage>(GetParamI(Param::Usage)); }
    inline void* GetMapPointer(Target target) const { return GetPointer(target, PtrParam::MapPointer); }
    inline void* GetMapPointer() const { return GetPointer(PtrParam::MapPointer); }
};

} // namespace O3GL