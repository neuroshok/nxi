#include <nxi/style.hpp>

#include <nxi/core.hpp>

#include <QString>
#include <QFile>

#include <w3c/theme.hpp>
#include <include/nxi/style.hpp>
#include <qdebug.h>

#include <ui/command/input.hpp>
#include <ui/command/menu.hpp>
#include <ui/interface/main.hpp>
#include <ui/interface/standard/control_bar.hpp>
#include <include/w3c/theme.hpp>
#include <include/nxi/log.hpp>

#include <QBrush>
#include <QLineEdit>
#include <QStyleOption>
#include <include/nxw/icon_button.hpp>
#include <include/nxi/style_data.hpp>
#include <QApplication>
#include <include/nxw/menu/item.hpp>
#include <include/nxw/menu.hpp>

namespace nxi
{
    style::style(const QString& name)
        : name_{ name }
        , path_{ nxi::core::module_path() + "/theme/"+ name_ }
    {}
    style::style() : style("nxi") {}

    const QString& style::name() const { return name_; }
    const QString& style::path() const { return path_; }
    const nxi::style_data& style::data() const { return data_; }

    void style::load()
    {
        QFile module_file(path() + "/manifest.json");
        if (!module_file.open(QFile::ReadOnly)) nxi_error("read error : {}", module_file.fileName());
        else
        {
            QJsonDocument doc = QJsonDocument::fromJson(module_file.readAll());
            QJsonObject obj = doc.object();
            module_file.close();

            w3c::theme w3c_theme{ obj["theme"] };
            from_w3c(w3c_theme);
        }
    }


    // findChildren require Q_OBJECT to be present on the searched type to avoid conversion with common base
    void style::update(ui::main_interface* w) const
    {
        w->style_data.background_color = data_.background_color.get();
        w->style_data.background_image = data_.background_image.get();
        w->style_data.background_image = w->style_data.background_image.transformed(QTransform().rotate(90));

        const QWidgetList top_widgets = QApplication::topLevelWidgets();
        for (auto top_widget : top_widgets)
        {
            for (auto command_input : top_widget->findChildren<ui::command_input*>()) update(command_input);
            for (auto command_menu : top_widget->findChildren<ui::command_menu*>()) update(command_menu);

            for (auto icon_button : top_widget->findChildren<nxw::icon_button*>())
            {
                icon_button->setFixedSize(data_.icon_button.size.get());
                icon_button->style_data.icon_color = data_.icon_button.icon_color.get();
                icon_button->style_data.icon_color_hover = data_.icon_button.icon_color_hover.get();
                icon_button->style_data.background_color = data_.icon_button.background_color.get();
                icon_button->style_data.background_color_hover = data_.icon_button.background_color_hover.get();
            }
        }
    }

    void style::update(ui::command_input* ui) const
    {
        ui->style_data.background_color = data_.field.background_color.get();
        ui->style_data.background_color_focus = data_.field.background_color_focus.get();
        ui->style_data.selection_highlight = data_.field.selection_highlight.get();
        ui->style_data.selection_highlight_text = data_.field.selection_highlight_text.get();
        ui->style_data.text_color = data_.field.text_color.get();
        ui->style_data.text_color_focus = data_.field.text_color_focus.get();
        ui::command_input::style_update(ui);
    }


    void style::update(QWidget* ui) const
    {
    }

    void style::update(ui::command_menu* widget) const
    {
        widget->style_data.background_image = data_.background_image.get();
        widget->style_data.background_color = data_.menu.background_color.get();
        widget->style_data.item_text_color = data_.menu.item_text_color.get();
        widget->style_data.item_text_color_hover = data_.menu.item_text_color_hover.get();
        widget->style_data.item_background_color = data_.menu.item_background_color.get();
        widget->style_data.item_background_color_hover = data_.menu.item_background_color_hover.get();
        widget->style_data.item_background_color_selected = data_.menu.item_background_color_selected.get();

        widget->style_data.item_height = 40;//data_.menu.item_height.get();
    }

    #define map_color(NXI_KEY, W3C_KEY) if (!W3C_KEY.is_null()) NXI_KEY.set(W3C_KEY.get());
    #define map_color_edit(NXI_KEY, W3C_KEY, EDIT) if (!W3C_KEY.is_null()) NXI_KEY.set(W3C_KEY.get().EDIT);
    //! \brief map w3c theme color to nxi style color
    void style::from_w3c(w3c::theme& theme)
    {
        data_.background_image.set(QImage(path() + "/" + theme.images.theme_frame.get()));
        map_color(data_.background_color, theme.colors.frame);

        map_color(data_.field.background_color, theme.colors.toolbar_field);
        map_color(data_.field.background_color_focus, theme.colors.toolbar_field_focus);
        map_color(data_.field.selection_highlight, theme.colors.toolbar_field_highlight);
        map_color(data_.field.selection_highlight_text, theme.colors.toolbar_field_highlight_text);
        map_color(data_.field.text_color, theme.colors.toolbar_field_text);
        map_color(data_.field.text_color_focus, theme.colors.toolbar_field_text_focus);

        map_color(data_.icon_button.background_color, theme.colors.toolbar_field);
        map_color(data_.icon_button.icon_color, theme.colors.popup_text);

        map_color(data_.field.background_color, theme.colors.toolbar_field);
        map_color(data_.field.background_color_focus, theme.colors.toolbar_field_focus);

        map_color(data_.menu.background_color, theme.colors.popup);
        map_color_edit(data_.menu.item_background_color, theme.colors.popup, darker(120));
        map_color(data_.menu.item_background_color_hover, theme.colors.popup_highlight);
        map_color_edit(data_.menu.item_background_color_selected, theme.colors.popup_highlight, darker(120));

        map_color(data_.menu.item_text_color_hover, theme.colors.popup_highlight_text);
        map_color(data_.menu.item_text_color, theme.colors.popup_text);
    }
    #undef map_color

////////////////////////////////////////////////////////////////////////////////
////////////////////////           BASIC STYLE          ////////////////////////
////////////////////////////////////////////////////////////////////////////////
    void style::polish(QPalette& palette)
    {
        QPainter painter;
        palette.setColor(QPalette::Text, data_.text_color.get());
        palette.setColor(QPalette::ColorRole::Base, Qt::transparent);
    }

    void style::drawPrimitive(QStyle::PrimitiveElement element, const QStyleOption* option, QPainter* painter,
                              const QWidget* widget) const
    {

        switch(element)
        {
            case QStyle::PE_PanelLineEdit:
            {
                int padding = 10;
                if (option->state & State_Editing)
                {
                    padding = 0;
                }

                QColor field_background_color{ data_.field.background_color.get() };
                if (option->state & State_HasFocus)
                {
                    field_background_color = data_.field.background_color_focus.get();
                }

                QPoint          topLeft     = option->rect.topLeft();
                QPoint          bottomRight = option->rect.topRight();
                QLinearGradient backgroundGradient(topLeft, bottomRight);
                backgroundGradient.setColorAt(0.0, field_background_color);
                backgroundGradient.setColorAt(1.0, field_background_color);

                painter->fillRect(option->rect, backgroundGradient);
                /*
                painter->setRenderHint(QPainter::Antialiasing);
                QPainterPath path;
                path.addRoundedRect(0, 0, option->rect.width(), option->rect.height(), 5, 5);

                painter->fillPath(path, backgroundGradient);
                painter->drawPath(path);*/

            }
            break;

            default:
                QProxyStyle::drawPrimitive(element, option, painter, widget);
        }
    }
} // nxi