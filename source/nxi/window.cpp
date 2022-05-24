#include <nxi/window.hpp>

#include <nxi/core.hpp>

namespace nxi
{
    window::window(nxi::core& core, nxi::window_data data)
        : core_{ core }
        , data_{ data }
        , buffer_system_{ core }
    {}

    int window::id() const { return data_.id; }
    int window::x() const { return data_.x; }
    int window::y() const { return data_.y; }
    int window::width() const { return data_.w; }
    int window::height() const { return data_.h; }

    nxi::buffer_system& window::buffer_system() { return buffer_system_; }
} // nxi