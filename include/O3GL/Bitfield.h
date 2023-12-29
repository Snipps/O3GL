#pragma once

#include <type_traits>
#include <initializer_list>

namespace O3GL {

template<typename T>
struct Bitfield
{
    using Value = typename std::underlying_type_t<T>;
    Value v;

    inline Bitfield():
        v(static_cast<Value>(0))
    {}

    inline Bitfield(T value):
        v(static_cast<Value>(value))
    {}

    inline Bitfield(Value value):
        v(value)
    {}

    inline Bitfield(std::initializer_list<T> list):
        v(static_cast<Value>(0))
    {
        for (T x : list)
            v |= static_cast<Value>(x);
    }

    inline Bitfield(const Bitfield&) = default;
    inline Bitfield(Bitfield&&) = default;
    inline Bitfield& operator=(const Bitfield&) = default;
    inline Bitfield& operator=(Bitfield&&) = default;
    inline ~Bitfield() = default;

    inline Bitfield& operator=(T other) { v = static_cast<Value>(other); return *this; }
    inline bool operator==(Bitfield other) const { return v == other.v; }
    inline bool operator==(T other) const { return v == static_cast<Value>(other); }
    inline bool operator!=(Bitfield other) const { return v != other.v; }
    inline bool operator!=(T other) const { return v != static_cast<Value>(other); }
    inline Bitfield operator|(T other) const { return v | static_cast<Value>(other); }
    inline Bitfield operator|(Bitfield other) const { return v | other.v; }
    inline Bitfield& operator|=(T other) { v |= static_cast<Value>(other); return *this; }
    inline Bitfield& operator|=(Bitfield other) { v |= other.v; return *this; }
    inline bool operator&(T bit) const { return v & static_cast<Value>(bit); }

    inline operator bool() const { return v != static_cast<Value>(0); }
    inline operator Value() const { return v; }
};

} // namespace O3GL