#include <platform/generic/notification.hpp>

#include <nxi/notification_data.hpp>

#include <QMessageBox>

namespace platform::generic
{
    notification::notification(const nxi::notification_data& data)
    {
        auto title = "[" + nxi::notification_data::to_string<QString>(data.type) + "] " + data.source;
        auto* error = new QMessageBox;
        error->setAttribute(Qt::WA_DeleteOnClose, true);
        error->setWindowTitle(title);
        error->setText(data.title + "\n\n" + data.message);
        error->show();
    }

    void notification::show() {}
} // platform::generic