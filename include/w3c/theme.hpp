#ifndef INCLUDE_W3C_THEME_HPP_NXI
#define INCLUDE_W3C_THEME_HPP_NXI


#include <regex>
#include <nxi/log.hpp>
#include <nxi/utility/json_loader.hpp>

#include <QColor>

struct color
{
    color() {}
    color(const QString& input)
    {
        if (input.size() > 0 && input[0] == '#')
        {
            QColor c{input};
            r = c.red();
            g = c.green();
            b = c.blue();
            a = c.alpha();
        }
        else
        {
            std::regex rgx(R"__(^rgba?\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+\.?\d*)\s*\)$)__");

            std::smatch match;
            auto str = input.toStdString();
            if (std::regex_search(str, match, rgx))
            {
                r = std::stoi(match[1]);
                g = std::stoi(match[2]);
                b = std::stoi(match[3]);
                a = std::stof(match[4]) * 255;
            } else nxi_warning("can't parse color '{}'", str);
        }
    }

    QColor toQColor() const
    {
        return QColor(r, g, b, a);
    }
    int r = 0;
    int g = 0;
    int b = 0;
    int a = 255;
};



//! https://developer.mozilla.org/en-US/docs/Mozilla/Add-ons/WebExtensions/manifest.json/theme

#include <QString>

namespace w3c
{
    struct theme : nxi::json_object
    {
        using json_object::json_object;

        nxi_json_open(images)
            nxi_json_open(nxi)
                nxi_json_key(nxi_value, QString)
            nxi_json_close(nxi)

            nxi_json_key(theme_frame, QString)
        nxi_json_close(images)

        nxi_json_open(colors)
            //!* The color of text and icons in the bookmark and find bars. Also, if tab_text isn't defined it sets
            //! the color of the active tab text and if icons isn't defined the color of the toolbar icons.
            //! Provided as Chrome compatible alias for toolbar_text.
            nxi_json_key(bookmark_text, ::color)
            nxi_json_key(button_background_active, ::color)
            nxi_json_key(button_background_hover, ::color)


            nxi_json_key(frame, ::color)
            nxi_json_key(tab_background_text, ::color)


            nxi_json_key(popup, ::color)
            nxi_json_key(popup_text, ::color)

            nxi_json_key(toolbar_field, ::color)
            nxi_json_key(toolbar_field_text, ::color)
            nxi_json_key(toolbar_text, ::color)
        nxi_json_close(colors)
    };
} // w3c

#endif // INCLUDE_W3C_THEME_HPP_NXI
