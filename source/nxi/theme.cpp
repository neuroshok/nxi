#include <nxi/theme.hpp>

#include <QString>

namespace nxi
{
    theme::theme() : name_{ "nxi" } {}
    theme::theme(const QString& name) : name_{ name } {}

    void theme::load()
    {
        QFile module_file("./module/theme/"+ name_ + "/manifest.json");
        if (!module_file.open(QFile::ReadOnly)) qDebug() << "read error : " << module_file.fileName();
        else
        {
            QJsonDocument doc = QJsonDocument::fromJson(module_file.readAll());
            QJsonObject obj = doc.object();
            module_file.close();

            data_ = new w3c::theme{ obj["theme"] };

            // manifest keys
            //auto theme = obj["theme"].toObject();
            //auto colors = theme["colors"].toObject();

            qDebug() <<  data_->colors.popup.get();
            qDebug() <<  data_->images.theme_frame.get();
            //qDebug() <<  th.colors.popup

            QPalette pal;
            pal.setColor(QPalette::ColorRole::Window, QColor(0, 0, 0, 255));
        }
    }

    void theme::apply(QWidget* widget) const
    {
        QEvent ev{ QEvent::PaletteChange };
        QApplication::sendEvent(widget, &ev);
    }

    void theme::update(ui::command* command) const
    {
        command->setFrame(false);
        QPalette pal;
        pal.setColor(QPalette::ColorRole::Base, QColor(22, 22, 22, 255));
        pal.setColor(QPalette::ColorRole::Text, QColor(200, 200, 200, 255));
        pal.setColor(QPalette::ColorRole::Highlight, QColor(0, 0, 150, 255));
        command->setPalette(pal);
        qDebug() << "update ";
    }
} // nxi