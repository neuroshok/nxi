#ifndef INCLUDE_NXI_COMMAND_SHORTCUT_INPUT_HPP_NXI
#define INCLUDE_NXI_COMMAND_SHORTCUT_INPUT_HPP_NXI

#include <nxi/command/fwd.hpp>

#include <unordered_set>
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

        void search(nxi::command_system&, QKeyEvent*);
        nxi::commands_view update(nxi::command_system&, QKeyEvent*);
        void untrigger();

    private:
        nxi::commands_view suggestions_;

        std::vector<Qt::Key> sequence_keys_;
        std::vector<Qt::Key> combo_keys_;

        std::vector<Qt::Key> input_;


        std::unordered_set<Qt::Key> triggers_;
        bool match_;
        trigger_mode trigger_mode_;
        int sequencing_;
        Qt::Key trigger_key_;
        int current_id_;
        bool executed_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_SHORTCUT_INPUT_HPP_NXI
