#pragma once

#include <type_traits>
#include <utility>

template<typename T>
class Movable {
public:
    static_assert(std::is_default_constructible_v<T>);
    static_assert(std::is_trivially_copy_constructible_v<T>);
    static_assert(std::is_trivially_copy_assignable_v<T>);
    static_assert(std::is_trivially_move_constructible_v<T>);
    static_assert(std::is_trivially_move_assignable_v<T>);
    static_assert(std::is_swappable_v<T>);

    Movable() = default;

    ~Movable() = default;

    explicit Movable(const T &val) noexcept { m_value = val; }

    Movable(const Movable &) = delete;

    Movable &operator=(const Movable &) = delete;

    Movable(Movable &&other) noexcept { m_value = std::exchange(other.m_value, {}); }

    Movable &operator=(Movable &&other) noexcept {
        std::swap(m_value, other.m_value);
        return *this;
    }

    operator const T &() const { // NOLINT(google-explicit-constructor)
        return m_value;
    }

    Movable &operator=(const T &val) {
        m_value = val;
        return *this;
    }

    T *operator&() // NOLINT(google-runtime-operator)
    {
        return &m_value;
    }

    const T *operator&() const { // NOLINT(google-runtime-operator)
        return &m_value;
    }

    T *operator->() { return &m_value; }

    const T *operator->() const { return &m_value; }

    bool operator==(const T &val) const { return m_value == val; }

    bool operator!=(const T &val) const { return m_value != val; }

private:
    T m_value{};
};

static_assert(sizeof(int) == sizeof(Movable<int>));

#define MOVABLE(T)                                                                                                                                   \
    T(const T &)                = delete;                                                                                                            \
    T &operator=(const T &)     = delete;                                                                                                            \
    T(T &&) noexcept            = default;                                                                                                           \
    T &operator=(T &&) noexcept = default;

#define NO_MOVE_OR_COPY(T)                                                                                                                           \
    T(const T &)            = delete;                                                                                                                \
    T &operator=(const T &) = delete;                                                                                                                \
    T(T &&)                 = delete;                                                                                                                \
    T &operator=(T &&)      = delete;