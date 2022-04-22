#ifndef INCLUDE_UI_INTERFACE_LIGHT_BUTTON_HPP_NXI
#define INCLUDE_UI_INTERFACE_LIGHT_BUTTON_HPP_NXI

#include <QLabel>

class QEvent;
class QWheelEvent;

namespace ui::interfaces::light
{
    class button : public QLabel
    {
        Q_OBJECT
    public:
        struct style_type
        {
            QColor background_color{ 150, 150, 150 };
            QColor highlight_color{ 200, 200, 200 };
            QColor text_color{ 0, 0, 0 };
        } style_data;

    public:
        button(const QString& name, QWidget* parent = nullptr);

        void activate(bool = true);

    protected:
        void enterEvent(QEnterEvent* event) override;
        void leaveEvent(QEvent* event) override;
        void wheelEvent(QWheelEvent* event) override;

    signals:
        void event_enter() const;
        void event_leave() const;
        void event_mousewheel_up() const;
        void event_mousewheel_down() const;

    private:
        bool active_ = false;
    };
} // nxi::interfaces::light

#endif // INCLUDE_UI_INTERFACE_LIGHT_BUTTON_HPP_NXI