#ifndef INCLUDE_NXI_SHORTCUT_HPP_NXI
#define INCLUDE_NXI_SHORTCUT_HPP_NXI

#include <vector>

#include <QWidget>
#include <QDebug>
#include <QKeyEvent>
#include <QListWidget>
#include <QLabel>
#include <QString>
#include <QChar>

namespace nxi
{
    struct shortcut
    {
        std::vector<Qt::Key> combo_keys;
        std::vector<Qt::Key> sequence_keys;

        bool is_empty() const
        {
            return combo_keys.empty() && sequence_keys.empty();
        }

        QString to_string() const
        {
            QString output;

            for (auto key : combo_keys) output += to_string(key) + " ";
            if (combo_keys.size() > 0) output += "+ ";
            for (auto key : sequence_keys) output += to_string(key) + " ";
            return output;
        }

        static QString to_string(Qt::Key key)
        {
            QString output;
            switch(key)
            {
                case Qt::Key_Alt:
                    output = "ALT";
                    break;
                case Qt::Key_Control:
                    output = "CTRL";
                    break;
                case Qt::Key_Shift:
                    output = "SHIFT";
                    break;
                default:
                    output = QKeySequence(key).toString();
            }
            return output;
        }
    };
} // nxi

#endif // INCLUDE_NXI_SHORTCUT_HPP_NXI
