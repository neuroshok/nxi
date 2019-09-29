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

        nxi_json_key(background_color, QColor, (61, 65, 71) )
        nxi_json_key(background_image, QImage, (":/style/background") )
        nxi_json_key(text_color, QColor, (186, 187, 193) )

        nxi_json_open(field)
            nxi_json_key(background_color, QColor, (34, 34, 34) )
            nxi_json_key(background_color_focus, QColor, (50, 50, 50) )
            nxi_json_key(text_color, QColor, (34, 34, 34) )
            nxi_json_key(text_color_focus, QColor, (34, 34, 34) )
        nxi_json_close(field)

        nxi_json_open(icon_button)
            nxi_json_key(background_color, QColor)
            nxi_json_key(icon_color, QColor, (238, 238, 238) )
            nxi_json_key(icon_color_hover, QColor, (0, 191, 255) )
            nxi_json_key(size, QSize, (24, 24) )
        nxi_json_close(icon_button)

        nxi_json_open(menu)
            nxi_json_key(background_color, QColor, (88, 91, 97) )
            nxi_json_key(item_text_color, QColor, (186, 187, 193) )
            nxi_json_key(item_text_color_hover, QColor, (186, 187, 193) )
            nxi_json_key(item_background_color_hover, QColor, (61, 65, 71) )
            nxi_json_key(item_height, int, (24) )
        nxi_json_close(menu)

        nxi_json_open(control_bar)
            nxi_json_open(command)
                nxi_json_key(nxi_value, QString)
            nxi_json_close(command)
        nxi_json_close(control_bar)
    };
} // nxi

#endif // INCLUDE_NXI_STYLE_DATA_HPP_NXI
