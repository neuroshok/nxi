#ifndef INCLUDE_NXI_WINDOW_HPP_NXI
#define INCLUDE_NXI_WINDOW_HPP_NXI

#include <nxi/data/window.hpp>
#include <nxi/system/buffer.hpp>

namespace nxi
{
    class core;

    class window
    {
    public:
        window(nxi::core& core, nxi::window_data data);

        [[nodiscard]] int id() const;
        [[nodiscard]] int x() const;
        [[nodiscard]] int y() const;
        [[nodiscard]] int width() const;

        [[nodiscard]] int height() const;
        [[nodiscard]] nxi::buffer_system& buffer_system();

    private:
        nxi::core& core_;
        nxi::window_data data_;

        nxi::buffer_system buffer_system_;
    };
} // nxi

#endif // INCLUDE_NXI_WINDOW_HPP_NXI