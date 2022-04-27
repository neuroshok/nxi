#ifndef INCLUDE_NXI_COMMAND_INPUT_HPP_NXI
#define INCLUDE_NXI_COMMAND_INPUT_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/command/shortcut_input.hpp>
#include <nxi/suggestion/vector.hpp>

#include <nds/graph/node.hpp>

#include <QObject>
#include <QString>

class QKeyEvent;

namespace nxi
{
    class command_system;
    class session;

    class command_input : public QObject
    {
        Q_OBJECT
    public:
        enum class mode_type {
            display //! display text when ui::command_input is not focus, input is shown on hover
            , input //! accept user input, can trigger shortcut mode
            , shortcut //! triggered by shortcut trigger_keys
        };

        enum class state { executing, search };

/*
        enum class context
        {
            command, page, history
        };*/

        command_input(nxi::session&);

        void exec();
        void update(const QString& input, QKeyEvent*);

        void context_suggest();
        void suggest_command();
        void suggest_context();
        void suggest_navigation();
        void suggest_page();

        void set_mode(mode_type);

        const QString& text() const;

        void clear();
        void reset();

        mode_type mode() const;
        const nxi::suggestion_vector& suggestions() const;
        nxi::suggestion_vector& suggestions();

        bool is_empty() const;
        bool is_valid() const;

        const nxi::command_system& command_system() const;
        nxi::command_system& command_system();
        nxi::shortcut_input& shortcut_input();

    signals:
        void event_suggestion_update(const nxi::suggestion_vector&);
        void event_input_update(const QString&);
        void event_selection_update(int index);
        void event_reset();

        void event_shortcut_input_update(const QString&);

    private:
        nxi::session& session_;

        QString input_;
        mode_type mode_;
        state state_;
        nxi::shortcut_input shortcut_input_;
        nxi::suggestion_vector suggestions_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_INPUT_HPP_NXI