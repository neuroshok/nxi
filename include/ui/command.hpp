#ifndef INCLUDE_UI_COMMAND_HPP_NXI
#define INCLUDE_UI_COMMAND_HPP_NXI

#include <QLineEdit>
#include <QKeyEvent>
#include <QLabel>
#include <QString>

namespace ui
{
    class command : public QLineEdit
    {
    public:
        command(ui::core& ui_core)
            : ui_core_{ ui_core }
        {
            info_ = new QLabel(this);
            menu_ = new nxw::menu(this);
            menu_->show_at(this);

            menu_->add_bottom(info_);
            menu_->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            menu_->hide();

            connect(this, &QLineEdit::editingFinished, [this]()
            {
                menu_->hide();
                menu_->clear();
            });

            connect(this, &QLineEdit::textChanged, [this]()
            {
                if (!hasFocus()) return;

                input_ = text();
                menu_->clear();
                auto result = ui_core_.nxi_core().command_system().search(input_);
                if (result.size() > 0)
                {
                    for (auto cmd : result)
                    {
                        menu_->add(*cmd);
                    }
                    menu_->show();
                    menu_->exec();
                }
                info_->setText("results : " + QString::number(result.size()));

                if (input_.isEmpty())
                {
                    auto focused_page = ui_core_.nxi_core().page_system().focus();
                    blockSignals(true);
                    if (focused_page.has_value()) setText(focused_page.value()->command());
                    selectAll();
                    blockSignals(false);
                }

            });

            connect(&ui_core_.nxi_core().page_system(), qOverload<nxi::page&>(&nxi::page_system::event_focus), this, [this](nxi::page& page)
            {
                setText(page.name());
            });

        }

        void resizeEvent(QResizeEvent* event) override
        {
            QLineEdit::resizeEvent(event);
            menu_->setFixedWidth(width());
        }

        void keyPressEvent(QKeyEvent* event) override
        {
            QLineEdit::keyPressEvent(event);
            if (event->key() == Qt::Key_Down)
            {
                emit textChanged(text());
            }
        }

        void focusOutEvent(QFocusEvent* event) override
        {
            QLineEdit::focusOutEvent(event);
            auto focused_page = ui_core_.nxi_core().page_system().focus();
            if (focused_page.has_value()) setText(focused_page.value()->name());
        }

        void enterEvent(QEvent* event) override
        {
            if (hasFocus()) return;
            if (!input_.isEmpty()) return;
            auto focused_page = ui_core_.nxi_core().page_system().focus();
            if (focused_page.has_value()) setText(focused_page.value()->command());
        }

        void leaveEvent(QEvent* event) override
        {
            if (hasFocus()) return;
            if (!input_.isEmpty()) return;
            auto focused_page = ui_core_.nxi_core().page_system().focus();
            if (focused_page.has_value()) setText(focused_page.value()->name());
        }

    private:
        ui::core& ui_core_;
        QString input_;

        nxw::menu* menu_;
        QLabel* info_;
    };
} // ui

#endif // INCLUDE_UI_COMMAND_HPP_NXI
