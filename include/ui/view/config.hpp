#ifndef INCLUDE_UI_VIEW_CONFIG_HPP_NXI
#define INCLUDE_UI_VIEW_CONFIG_HPP_NXI

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QFormLayout>

#include <ui/core.hpp>
#include <nxi/core.hpp>
#include <nxi/database/config.hpp>
#include <include/nxi/database/config.hpp>

namespace ui
{
    class config : public QWidget
    {
    public:
        config(ui::core& ui_core) : ui_core_{ ui_core }
        {
            auto& cfg = ui_core_.nxi_core().config();

            form_ = new QFormLayout(this);

            add_row(cfg.browser.home);
            add_row(cfg.browser.download_path);
            add_row(cfg.browser.history.preview_size);
            add_row(cfg.browser.history.suggest_ratio);
        }

        template<class T>
        void add_row(T& persistent)
        {
            auto value = new QLineEdit(this);
            QString str_value;

            auto persistent_process = [value, &str_value](auto& persistent)
            {
                using value_type = typename std::decay_t<decltype(persistent)>::value_type;
                if constexpr (std::is_same_v<value_type, std::string>)
                {
                    str_value = QString::fromStdString(persistent.get());
                    QObject::connect(value, &QLineEdit::returnPressed, [&persistent, value]() { persistent = value->text().toStdString(); });
                }
                else if constexpr(std::is_same_v<value_type, int>)
                {
                    str_value = QString::number(persistent.get());
                    QObject::connect(value, &QLineEdit::returnPressed, [&persistent, value]() { persistent = value->text().toInt(); });
                }
                else if constexpr(std::is_same_v<value_type, float>)
                {
                    str_value = QString::number(persistent.get());
                    QObject::connect(value, &QLineEdit::returnPressed, [&persistent, value]() { persistent = value->text().toFloat(); });
                }
            };

            persistent_process(persistent);

            value->setText(str_value);

            form_->addRow(persistent.path().c_str(), value);
        }

    private:
        ui::core& ui_core_;

        QFormLayout* form_;
    };
} // nxi

#endif // INCLUDE_UI_VIEW_CONFIG_HPP_NXI
