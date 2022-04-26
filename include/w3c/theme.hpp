#ifndef INCLUDE_W3C_THEME_HPP_NXI
#define INCLUDE_W3C_THEME_HPP_NXI


#include <regex>
#include <nxi/log.hpp>
#include <nxi/utility/json_loader.hpp>

#include <QColor>

namespace w3c
{
    struct color : public QColor
    {
        color() = default;
        color(const QString& input) : QColor(init_color(input))
        {

        }

        QColor init_color(const QString& input)
        {
            if (input[0] == 'r')
            {
                QColor color;
                std::regex rgx(R"__(^rgba?\(\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+)\s*,\s*(\d+\.?\d*)\s*\)$)__");

                std::smatch match;
                auto str = input.toStdString();
                if (std::regex_search(str, match, rgx))
                {
                    color.setRed(std::stoi(match[1]));
                    color.setGreen(std::stoi(match[2]));
                    color.setBlue(std::stoi(match[3]));
                    color.setAlpha(std::stof(match[4]) * 255);
                } else nxi_warning("can't parse color '{}'", str);
                return color;
            }
            return QColor(input);
        }
    };
} // w3c


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
            //! The color of text and icons in the bookmark and find bars. Also, if tab_text isn't defined it sets
            //! the color of the active tab text and if icons isn't defined the color of the toolbar icons.
            //! Provided as Chrome compatible alias for toolbar_text.
            nxi_json_key(bookmark_text, w3c::color)
            nxi_json_key(button_background_active, w3c::color)
            nxi_json_key(button_background_hover, w3c::color)


            nxi_json_key(frame, w3c::color)
            nxi_json_key(tab_background_text, w3c::color)

            //! The background color of popups (such as the url bar dropdown and the arrow panels).
            nxi_json_key(popup, w3c::color)
            nxi_json_key(popup_border, w3c::color)
            nxi_json_key(popup_highlight, w3c::color) //! The background color of items highlighted using the keyboard inside popups (such as the selected URL bar dropdown item).
            nxi_json_key(popup_highlight_text, w3c::color) //! The text color of items highlighted inside popups.
            nxi_json_key(popup_text, w3c::color)

            // ui::command_input
            nxi_json_key(toolbar_field, w3c::color)
            nxi_json_key(toolbar_field_focus, w3c::color)
            nxi_json_key(toolbar_field_highlight, w3c::color) //! The background color used to indicate the current selection of text in the URL bar
            nxi_json_key(toolbar_field_highlight_text, w3c::color) //! The color used to draw text that's currently selected in the URL bar
            nxi_json_key(toolbar_field_text, w3c::color) //! The color of text in fields in the toolbar, such as the URL bar. This also sets the color of text in the Find in page field.
            nxi_json_key(toolbar_field_text_focus, w3c::color) //! The color of text in focused fields in the toolbar, such as the URL bar.
            nxi_json_key(toolbar_text, w3c::color) //! The color of toolbar text. This also sets the color of text in the "Find" bar.
        nxi_json_close(colors)
    };
} // w3c

#endif // INCLUDE_W3C_THEME_HPP_NXI