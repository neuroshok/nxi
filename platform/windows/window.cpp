#include <platform/windows/window.hpp>

#include <QApplication>
#include <QPoint>
#include <QSize>
#include <QMouseEvent>
#include <QDebug>

#include <windows.h>
#include <WinUser.h>
#include <windowsx.h>
#include <dwmapi.h>

#pragma comment (lib,"Dwmapi.lib") // Adds missing library, fixes error LNK2019: unresolved external symbol __imp__DwmExtendFrameIntoClientArea
#pragma comment (lib,"user32.lib")

namespace platform::windows
{
    window::window(QWidget *parent)
        : QWidget(parent)
        , grip_{ nullptr }
        , mouse_down_{ false }
    {
        setWindowFlag(Qt::Window, true);
        setWindowFlag(Qt::FramelessWindowHint, true);
        setWindowFlag(Qt::WindowSystemMenuHint, true);

        // resizeable
        setWindowFlag(Qt::WindowMaximizeButtonHint);

        HWND hwnd = (HWND)winId();
        DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
        ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);

        const MARGINS shadow = { 1, 1, 1, 1 };
        DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);

        setVisible(true);
    }

    bool window::nativeEvent(const QByteArray& eventType, void* message, long* result)
    {
        // https://forum.qt.io/topic/93141/qtablewidget-itemselectionchanged/13
        #if (QT_VERSION == QT_VERSION_CHECK(5, 11, 1))
            MSG* msg = *reinterpret_cast<MSG**>(message);
        #else
            MSG* msg = reinterpret_cast<MSG*>(message);
        #endif

        switch (msg->message)
        {
            case WM_NCLBUTTONDOWN:
            {
                mouse_down_ = true;
                return false;
            }

            case WM_NCMOUSEMOVE:
            {
                if (mouse_down_)
                {
                    long x = GET_X_LPARAM(msg->lParam);
                    long y = GET_Y_LPARAM(msg->lParam);
                    *result = 0;
                    QMouseEvent event(QEvent::MouseButtonRelease, QPoint(x, y), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
                    emit mouseReleaseEvent(&event);
                    mouse_down_ = false;
                }
                return true;
            }

            case WM_NCCALCSIZE:
            {
                NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
                if (params.rgrc[0].top != 0) params.rgrc[0].top -= 1;

                // remove WS_THICKFRAME and WS_CAPTION
                *result = WVR_REDRAW;
                return true;
            }

            case WM_NCHITTEST:
            {
                *result = 0;

                const LONG border_width = 8;
                RECT winrect;
                ::GetWindowRect(HWND(winId()), &winrect);

                long x = GET_X_LPARAM(msg->lParam);
                long y = GET_Y_LPARAM(msg->lParam);

                bool resizeWidth = minimumWidth() != maximumWidth();
                bool resizeHeight = minimumHeight() != maximumHeight();

                if(resizeWidth)
                {
                    //left border
                    if (x >= winrect.left && x < winrect.left + border_width)
                    {
                        *result = HTLEFT;
                    }
                    //right border
                    if (x < winrect.right && x >= winrect.right - border_width)
                    {
                        *result = HTRIGHT;
                    }
                }
                if(resizeHeight)
                {
                    //bottom border
                    if (y < winrect.bottom && y >= winrect.bottom - border_width)
                    {
                        *result = HTBOTTOM;
                    }
                    //top border
                    if (y >= winrect.top && y < winrect.top + border_width)
                    {
                        *result = HTTOP;
                    }
                }
                if(resizeWidth && resizeHeight)
                {
                    //bottom left corner
                    if (x >= winrect.left && x < winrect.left + border_width &&
                            y < winrect.bottom && y >= winrect.bottom - border_width)
                    {
                        *result = HTBOTTOMLEFT;
                    }
                    //bottom right corner
                    if (x < winrect.right && x >= winrect.right - border_width &&
                            y < winrect.bottom && y >= winrect.bottom - border_width)
                    {
                        *result = HTBOTTOMRIGHT;
                    }
                    //top left corner
                    if (x >= winrect.left && x < winrect.left + border_width &&
                            y >= winrect.top && y < winrect.top + border_width)
                    {
                        *result = HTTOPLEFT;
                    }
                    //top right corner
                    if (x < winrect.right && x >= winrect.right - border_width &&
                            y >= winrect.top && y < winrect.top + border_width)
                    {
                        *result = HTTOPRIGHT;
                    }
                }
                if (*result != 0) return true;

                // grip region
                if (!grip_) return false;

                //support highdpi
                double dpr = this->devicePixelRatioF();
                QPoint pos = grip_->mapFromGlobal(QPoint(x / dpr, y / dpr));
                QWidget* child = grip_->childAt(pos);

                // ignore children or mouse out of region
                if (child || !grip_->rect().contains(pos)) return false;
                *result = HTCAPTION;
                return true;
            } // WM_NCHITTEST

        case WM_GETMINMAXINFO:
        {
            if (::IsZoomed(msg->hwnd))
            {
                RECT frame = { 0, 0, 0, 0 };
                AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);

                // record frame area data
                double dpr = this->devicePixelRatioF();

                m_frames.setLeft(abs(frame.left)/dpr+0.5);
                m_frames.setTop(abs(frame.bottom)/dpr+0.5);
                m_frames.setRight(abs(frame.right)/dpr+0.5);
                m_frames.setBottom(abs(frame.bottom)/dpr+0.5);

                QWidget::setContentsMargins(m_frames.left()+m_margins.left(), \
                                                m_frames.top()+m_margins.top(), \
                                                m_frames.right()+m_margins.right(), \
                                                m_frames.bottom()+m_margins.bottom());
                m_bJustMaximized = true;
            }
            else
            {
                if (m_bJustMaximized)
                {
                    QWidget::setContentsMargins(m_margins);
                    m_frames = QMargins();
                    m_bJustMaximized = false;
                }
            }
            return false;
        }
        default:
            return QWidget::nativeEvent(eventType, message, result);
        }
    }

    void window::setContentsMargins(const QMargins &margins)
    {
        QWidget::setContentsMargins(margins+m_frames);
        m_margins = margins;
    }
    void window::setContentsMargins(int left, int top, int right, int bottom)
    {
        QWidget::setContentsMargins(left+m_frames.left(),\
                                        top+m_frames.top(), \
                                        right+m_frames.right(), \
                                        bottom+m_frames.bottom());
        m_margins.setLeft(left);
        m_margins.setTop(top);
        m_margins.setRight(right);
        m_margins.setBottom(bottom);
    }
    QMargins window::contentsMargins() const
    {
        QMargins margins = QWidget::contentsMargins();
        margins -= m_frames;
        return margins;
    }
    void window::getContentsMargins(int *left, int *top, int *right, int *bottom) const
    {
        QWidget::getContentsMargins(left,top,right,bottom);
        if (!(left&&top&&right&&bottom)) return;
        if (isMaximized())
        {
            *left -= m_frames.left();
            *top -= m_frames.top();
            *right -= m_frames.right();
            *bottom -= m_frames.bottom();
        }
    }

    QRect window::contentsRect() const
    {
        QRect rect = QWidget::contentsRect();
        int width = rect.width();
        int height = rect.height();
        rect.setLeft(rect.left() - m_frames.left());
        rect.setTop(rect.top() - m_frames.top());
        rect.setWidth(width);
        rect.setHeight(height);
        return rect;
    }

    void window::set_fullscreen()
    {
        if (isMaximized())
        {
            QWidget::setContentsMargins(m_margins);
            m_frames = QMargins();
        }
        QWidget::showFullScreen();
    }

    void window::set_grip(QWidget* widget)
    {
        grip_ = widget;
    }

    /*
    void window::mousePressEvent(QMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton)
        {
            if ( event->type() == QEvent::MouseButtonDblClick)
            {
                qDebug() << "window::MouseButtonDblClick";
                showMaximized();
            }

            //SetCapture(winId());
            ReleaseCapture();
            SendMessage( (HWND)winId(), WM_NCLBUTTONDOWN, HTCAPTION, 0 );
            SetCapture((HWND)winId());

            //qDebug() << "window::mousePressEvent";

        }
    }


    void window::mouseReleaseEvent(QMouseEvent *event)
    {

        SendMessage( (HWND)winId(), WM_NCLBUTTONUP, HTCAPTION, 0 );
        qDebug() << "window::mouseReleaseEvent";
    }

    void window::mouseMoveEvent(QMouseEvent *event)
    {

        QWidget::mouseMoveEvent(event);
    }*/


} // platform::windows
