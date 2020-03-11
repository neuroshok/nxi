#ifndef INCLUDE_NXI_COMMAND_INPUT_HPP_NXI
#define INCLUDE_NXI_COMMAND_INPUT_HPP_NXI

#include <nxi/command/fwd.hpp>
#include <nxi/command/params.hpp>
#include <nxi/command/shortcut_input.hpp>
#include <nxi/suggestion/vector.hpp>

#include <nds/graph/node.hpp>

#include <QObject>
#include <QString>

class QKeyEvent;

namespace nxi
{
    class command_system;
    class core;

    class command_input : public QObject
    {
        Q_OBJECT
    public:
        enum class states { command, command_param, shortcut };

        command_input(nxi::core&);

        void add_param(const QString& param);

        void update(const QString& input, QKeyEvent*);
        void exec();

        void set_input(const QString&);

        states state() const;
        QString state_text() const;

        const nxi::command_params& params();
        const nxi::suggestion_vector& suggestions() const;
        const QString& text() const;

        void suggest_command();
        void suggest_page();

        void select_suggestion(int index);
        void select_previous_suggestion();
        void select_next_suggestion();
        size_t suggestion_count() const;
        const suggestion_vector::suggestion_type& suggestion(int index);
        const suggestion_vector::suggestion_type& selected_suggestion();
        int selected_suggestion_index();
        bool has_selected_suggestion();

        void clear();
        void reset();

        bool is_empty() const;
        bool is_valid() const;

        nxi::command_system& command_system();
        nxi::shortcut_input& shortcut_input();

    signals:
        void event_suggestion_update(const nxi::suggestion_vector&);
        void event_input_update(const QString&);
        void event_state_update(states);
        void event_selection_update(int index);
        void event_reset();

        void event_shortcut_input_update(const QString&);
        void event_command_param_required(const QString&);

    protected:
        void set_state(states);

    private:
        nxi::core& nxi_core_;
        nxi::command_system& command_system_;

        nxi::shortcut_input shortcut_input_;
        QString input_;
        states state_;
        nxi::command_params params_;
        nds::node_ptr<const nxi::command> command_;
        int param_index_ = 0;

        int selected_suggestion_index_;
        nxi::suggestion_vector suggestions_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_INPUT_HPP_NXI
