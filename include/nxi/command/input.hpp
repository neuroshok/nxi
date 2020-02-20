#ifndef INCLUDE_NXI_COMMAND_INPUT_HPP_NXI
#define INCLUDE_NXI_COMMAND_INPUT_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/command/params.hpp>
#include <nxi/command/shortcut_input.hpp>

#include <QObject>
#include <QString>

class QKeyEvent;

namespace nxi
{
    class command_system;

    class command_input : public QObject
    {
        Q_OBJECT
    public:
        enum class states { command, action, param };

        command_input(nxi::command_system& command_system);

        void add_param(const QString& param);

        void update(const QString& input, QKeyEvent*);
        void exec();

        states state() const;
        QString state_text() const;

        const nxi::command_params& params();
        std::vector<stz::observer_ptr<nxi::command>> suggestions() const;
        const std::vector<QString>& param_suggestions() const;
        const QString& text() const;

        void suggest_command();
        void select_suggestion(int index);
        void select_previous_suggestion();
        void select_next_suggestion();
        size_t suggestion_count() const;
        stz::observer_ptr<nxi::command> suggestion(int index);
        stz::observer_ptr<nxi::command> selected_suggestion();
        int selected_suggestion_index();
        bool has_selected_suggestion();

        void clear();
        void reset();

        bool is_empty() const;
        bool is_valid() const;

        nxi::shortcut_input& shortcut_input();

    signals:
        void event_suggestion_update(stz::observer_ptr<nxi::commands_view>);
        //void event_suggestion_update(std::vector<stz::observer_ptr<nxi::page>>);
        void event_state_update(states);
        void event_selection_update(int index);
        void event_complete();

        void event_shortcut_input_update(const QString&);

    private:
        nxi::command_system& command_system_;

        nxi::shortcut_input shortcut_input_;
        QString input_;
        states state_;
        nxi::command_params params_;
        stz::observer_ptr<nxi::command> command_;
        int param_index_ = 0;

        int selected_suggestion_index_;
        nxi::commands_view suggestions_;
        std::vector<QString> param_suggestions_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_INPUT_HPP_NXI
