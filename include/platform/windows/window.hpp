#ifndef INCLUDE_PLATFORM_WINDOWS_WINDOW_HPP_NXI
#define INCLUDE_PLATFORM_WINDOWS_WINDOW_HPP_NXI

#include <QObject>
#include <QWidget>

namespace platform::windows
{
    class window : public QWidget
    {
        Q_OBJECT
    public:
        explicit window(QWidget* parent = 0);
        void set_grip(QWidget* widget);

        void setContentsMargins(const QMargins& margins);
        void setContentsMargins(int left, int top, int right, int bottom);
        QMargins contentsMargins() const;
        QRect contentsRect() const;
        void getContentsMargins(int* left, int* top, int* right, int* bottom) const;

        void set_fullscreen();

    protected:
        bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;

    private:
        QWidget* grip_;
        bool mouse_down_;

        QMargins m_margins;
        QMargins m_frames;
        bool m_bJustMaximized;
    };
} // platform

#endif // INCLUDE_PLATFORM_WINDOWS_WINDOW_HPP_NXI