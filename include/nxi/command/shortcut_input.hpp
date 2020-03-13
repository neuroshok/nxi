#ifndef INCLUDE_NXI_COMMAND_SHORTCUT_INPUT_HPP_NXI
#define INCLUDE_NXI_COMMAND_SHORTCUT_INPUT_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/shortcut.hpp>

#include <unordered_set>
#include <QKeyEvent>

namespace nxi
{
    class command_system;
    class command_input;
    class suggestion_vector;

    class shortcut_input
    {
    public:
        enum class trigger_mode{ combo, sequence };

        shortcut_input(const nxi::command_input&);

        void search(Qt::Key, nxi::suggestion_vector&);
        void update(QKeyEvent*, nxi::suggestion_vector&);
        void reset();

        void add_trigger_key(Qt::Key);
        bool is_trigger_key(Qt::Key) const;
        bool is_triggered() const;

        QString to_string() const;

    signals:
        //void input_update(QString keys);

    private:
        const nxi::command_input& command_input_;
        nxi::shortcut input_;
        std::unordered_set<Qt::Key> trigger_keys_;

        bool is_triggered_;
        trigger_mode trigger_mode_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_SHORTCUT_INPUT_HPP_NXI
