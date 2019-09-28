#include <nxi/style.hpp>

#include <QString>
#include <QFile>

#include <w3c/theme.hpp>
#include <include/nxi/style.hpp>
#include <qdebug.h>

#include <ui/command.hpp>
#include <ui/interface/main.hpp>
#include <ui/interface/control_bar.hpp>
#include <include/w3c/theme.hpp>
#include <include/nxi/log.hpp>

#include <QBrush>
#include <QLineEdit>
#include <include/nxw/icon_button.hpp>
#include <include/nxi/style_data.hpp>
#include <QApplication>
#include <include/nxw/menu/item.hpp>
#include <include/nxw/menu.hpp>

namespace nxi
{
    style::style(const QString& name)
        : name_{ name }
        , path_{ "./module/theme/"+ name_ + "/" }
    {}
    style::style() : style("nxi") {}

    const QString& style::name() const { return name_; }
    const QString& style::path() const { return path_; }
    const nxi::style_data& style::data() const { return data_; }

    void style::load()
    {
        QFile module_file(path() + "manifest.json");
        if (!module_file.open(QFile::ReadOnly)) nxi_error("read error : {}", module_file.fileName());
        else
        {
            QJsonDocument doc = QJsonDocument::fromJson(module_file.readAll());
            QJsonObject obj = doc.object();
            module_file.close();

            w3c::theme w3c_theme{ obj["theme"] };
            from_w3c(w3c_theme);
        }

        const QWidgetList top_widgets = QApplication::topLevelWidgets();
        for (auto top_widget : top_widgets)
        {
            for (auto widget : top_widget->findChildren<nxw::menu*>())
            {
                update(widget);
            }

            for (auto w : top_widget->findChildren<nxw::icon_button*>())
            {
                w->setFixedSize(data_.icon_button.size.get());
                w->style.icon_color = data_.icon_button.icon_color.get();
                w->style.icon_color_hover = data_.icon_button.icon_color_hover.get();
            }

            for (auto w : top_widget->findChildren<ui::interfaces::main*>())
            {
                w->style_data.background_color = data_.background_color.get();
                w->style_data.background_image = data_.background_image.get();
                w->style_data.background_image = w->style_data.background_image.transformed(QMatrix().rotate(90));
            }
        }
    }


    void style::update(ui::interfaces::main* ui) const
    {


    }

    void style::update(ui::command* ui) const
    {

    }

    void style::update(ui::interfaces::control_bar* ui) const
    {

    }

    void style::update(QWidget* ui) const
    {

    }

    void style::update(ui::views::page_tree* ui) const
    {
    }

    void style::update(nxw::menu* widget)
    {
        widget->style_data.background_color = data_.menu.background_color.get();
        widget->style_data.item_text_color = data_.menu.item_text_color.get();
        widget->style_data.item_text_color_hover = data_.menu.item_text_color_hover.get();
        widget->style_data.item_background_color_hover = data_.menu.item_background_color_hover.get();
        widget->style_data.item_background_color_hover = data_.menu.item_background_color_hover.get();

        widget->style_data.item_height = data_.menu.item_height.get();
    }

    #define map_color(W3C_KEY, NXI_KEY) if (!W3C_KEY.is_null()) NXI_KEY.set(W3C_KEY.get());
    void style::from_w3c(w3c::theme& theme)
    {
        data_.background_image.set(QImage(path() + theme.images.theme_frame.get()));

        map_color(theme.colors.popup, data_.menu.background_color);
        //map_color(theme.colors.popup_border, data_.menu.background_color);
        map_color(theme.colors.popup_highlight, data_.menu.item_background_color_hover);
        map_color(theme.colors.popup_highlight_text, data_.menu.item_text_color_hover);
        map_color(theme.colors.popup_text, data_.menu.item_text_color);
    }
    #undef map_color

////////////////////////////////////////////////////////////////////////////////
////////////////////////           BASIC STYLE          ////////////////////////
////////////////////////////////////////////////////////////////////////////////
    void style::polish(QPalette& palette)
    {
        QPainter painter;
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
                backgroundGradient.setColorAt(1.0, QColor(0, 0, 0));

                painter->setRenderHint(QPainter::Antialiasing);
                QPainterPath path;

                path.addRoundedRect(0, 0, option->rect.width(), option->rect.height(), 5, 5);

                painter->fillPath(path, backgroundGradient);
                painter->drawPath(path);

            }
            break;

            default:
                QProxyStyle::drawPrimitive(element, option, painter, widget);
        }
    }
} // nxi