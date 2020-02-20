#ifndef INCLUDE_NXI_COMMAND_SHORTCUT_INPUT_HPP_NXI
#define INCLUDE_NXI_COMMAND_SHORTCUT_INPUT_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/shortcut.hpp>

#include <vector>
#include <QKeyEvent>

namespace nxi
{
    class command_system;

    class shortcut_input
    {
    public:
        enum class trigger_mode{ combo, sequence };

        shortcut_input();

        void search(nxi::command_system&, Qt::Key);
        nxi::commands_view update(nxi::command_system&, QKeyEvent*);
        void untrigger();
        QString to_string();

    signals:
        //void input_update(QString keys);

    private:
        nxi::commands_view suggestions_;
        nxi::shortcut input_;

        trigger_mode trigger_mode_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_SHORTCUT_INPUT_HPP_NXI
