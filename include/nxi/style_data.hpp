#ifndef INCLUDE_NXI_STYLE_DATA_HPP_NXI
#define INCLUDE_NXI_STYLE_DATA_HPP_NXI

#include <nxi/utility/json_loader.hpp>

#include <QColor>
#include <QSize>

namespace nxi
{
    struct style_data : nxi::json_object
    {
        using json_object::json_object;
        nxi_json_open(icon_button)
            nxi_json_key(background_color, QColor)
            nxi_json_key(icon_color, QColor)
            nxi_json_key(icon_color_hover, QColor)
            nxi_json_key(size, QSize)
        nxi_json_close(icon_button)

        nxi_json_open(control_bar)
            nxi_json_open(command)
                nxi_json_key(nxi_value, QString)
            nxi_json_close(command)
        nxi_json_close(control_bar)
    };
} // nxi

#endif // INCLUDE_NXI_STYLE_DATA_HPP_NXI
