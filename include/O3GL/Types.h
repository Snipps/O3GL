#pragma once

#include <cstdint>

namespace O3GL {

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef int8_t GLbyte;
typedef uint8_t GLubyte;
typedef int16_t GLshort;
typedef uint16_t GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef int32_t GLclampx;
typedef int GLsizei;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef char GLchar;
typedef uint16_t GLhalf;
typedef int32_t GLfixed;
typedef intptr_t GLintptr;
typedef intptr_t GLsizeiptr;
typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef struct __GLsync *GLsync;
typedef void (*GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message,const void *userParam);

enum class Type : GLenum
{
    Byte = 0x1400,
    UByte = 0x1401,
    Short = 0x1402,
    UShort = 0x1403,
    Int = 0x1404,
    UInt = 0x1405,
    Float = 0x1406,
    Double = 0x140A,
    Half = 0x140B,
    Fixed = 0x140C
};

enum class Format : GLenum
{
    R = 0x1903,
    RG = 0x8227,
    RGB = 0x1907,
    RGBA = 0x1908
};

enum class SizedInternalFormat : GLenum
{
    // R
    R8 = 0x8229,
    R16 = 0x822A,
    R16F = 0x822D,
    R32F = 0x822E,
    R8I = 0x8231,
    R16I = 0x8233,
    R32I = 0x8235,
    R8UI = 0x8232,
    R16UI = 0x8234,
    R32UI = 0x8236,

    // RG
    RG8 = 0x822B,
    RG16 = 0x822C,
    RG16F = 0x822F,
    RG32F = 0x8230,
    RG8I = 0x8237,
    RG16I = 0x8239,
    RG32I = 0x823B,
    RG8UI = 0x8238,
    RG16UI = 0x823A,
    RG32UI = 0x823C,

    // RGB
    RGB16F = 0x881B,
    RGB32F = 0x8815,
    RGB8I = 0x8D8F,
    RGB16I = 0x8D89,
    RGB32I = 0x8D83,
    RGB8UI = 0x8D7D,
    RGB16UI = 0x8D77,
    RGB32UI = 0x8D71,

    // RGBA
    RGBA8 = 0x8058,
    RGBA16 = 0x805B,
    RGBA16F = 0x881A,
    RGBA32F = 0x8814,
    RGBA8I = 0x8D8E,
    RGBA16I = 0x8D88,
    RGBA32I = 0x8D82,
    RGBA8UI = 0x8D7C,
    RGBA16UI = 0x8D76,
    RGBA32UI = 0x8D70
};

} // namespace O3GL