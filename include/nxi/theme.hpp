#ifndef INCLUDE_NXI_THEME_HPP_NXI
#define INCLUDE_NXI_THEME_HPP_NXI

#include <QString>
#include <QFile>
#include <QDebug>
#include <QApplication>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonValueRef>

#include <QWidget>
#include <include/ui/command.hpp>

struct json_value;

struct json_object
{
    friend struct json_value;
public:
    json_object(QJsonValueRef value) : value_{ value }{}
    json_object(json_object* parent, const QString& key)
        : value_{ parent->value_.toObject()[key] }
    {}
private:
    QJsonValue value_;
};

struct json_value
{
public:
    json_value(json_object* parent, const QString& key)
        : value_{ parent->value_.toObject()[key].toString() }
    {}

    const QString& get() const { return value_; }
private:
    QString value_;
};




namespace w3c
{
    struct theme : json_object
    {
        using json_object::json_object;
        using color_type = json_value;

        struct : json_object
        {
            using json_object::json_object;

            struct : json_object
            {
                using json_object::json_object;
                theme::color_type nxi_value{ this, "nxi_value" };
            } nxi{ this, "nxi" };

            theme::color_type theme_frame{ this, "theme_frame" };
        } images{ this, "images" };

        struct : json_object
        {
            using json_object::json_object;

            theme::color_type button_background_hover{ this, "button_background_hover" };
            theme::color_type popup{ this, "popup" };
            theme::color_type popup_text{ this, "popup_text" };
        } colors{ this, "colors" };


    };
} // w3c

namespace nxi
{
    class theme
    {
    public:
        theme();
        theme(const QString& name);

        void load();

        void apply(QWidget* widget) const;

        void update(ui::command* command) const;

    private:
        QString name_;
        w3c::theme* data_;
    };
} // nxi
/*
 * "tab_background_text": "white",
     "popup_highlight": "black",
     "popup_highlight_text": "red"
 *
 */


#endif // INCLUDE_NXI_THEME_HPP_NXI
