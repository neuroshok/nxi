#ifndef INCLUDE_UI_VIEW_CONFIG_HPP_NXI
#define INCLUDE_UI_VIEW_CONFIG_HPP_NXI

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>

#include <ui/system/session.hpp>
#include <nxi/core.hpp>
#include <nxi/config.hpp>
#include <nxi/utility.hpp>

namespace ui::views
{
    class config : public QWidget
    {
    public:
        config(ui::session& session);

        template<class T>
        void add_row(nxi::persistent<T>* persistent)
        {
            auto config_key = static_cast<nxi::config_key<T>*>(persistent);

            auto value = new QLineEdit(this);
            value->setStyleSheet("background-color: #888888;");
            QString str_value;

            auto config_key_process = [&value, &str_value](nxi::persistent<T>* config_key)
            {
                if constexpr (std::is_same_v<std::decay_t<T>, QString>)
                {
                    str_value = config_key->get();
                    connect(value, &QLineEdit::returnPressed, [config_key, value]() { *config_key = value->text(); });
                }
                else if constexpr(std::is_same_v<T, int> || std::is_same_v<T, bool>)
                {
                    str_value = QString::number(config_key->get());
                    connect(value, &QLineEdit::returnPressed, [config_key, value]() { *config_key = value->text().toInt(); });
                }
                else if constexpr(std::is_same_v<T, double>)
                {
                    str_value = QString::number(config_key->get());
                    connect(value, &QLineEdit::returnPressed, [config_key, value]() { *config_key = value->text().toDouble(); });
                }
                else nxi_error("nxi::config field error");
            };

            config_key_process(config_key);

            value->setText(str_value);

            auto label_key = new QLabel(this);
            label_key->setText(config_key->path());
            label_key->setToolTip(config_key->description());

            form_->addRow(label_key, value);

        }

    private:
        ui::session& session_;

        QFormLayout* form_;
    };
} // nxi::views

#endif // INCLUDE_UI_VIEW_CONFIG_HPP_NXI