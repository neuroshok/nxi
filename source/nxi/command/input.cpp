#include <nxi/command/input.hpp>

#include <nxi/command.hpp>
#include <nxi/core.hpp>
#include <nxi/log.hpp>
#include <nxi/system/command.hpp>

#include <QKeyEvent>

namespace nxi
{

    command_input::command_input(nxi::core& nxi_core)
        : nxi_core_{ nxi_core }
        , command_system_{ nxi_core_.command_system() }
        , shortcut_input_{ *this }
        , state_{ states::command }
        , command_{ nullptr }
        , selected_suggestion_index_{ -1 }
    {
        connect(&command_system_, &nxi::command_system::event_param_required, [this](nds::node_ptr<const nxi::command> command)
        {
            reset();
            command_ = command;
            set_state(states::command_param);
        });
    }

    void command_input::add_param(const QString& param)
    {

    }

    void command_input::update(const QString& input, QKeyEvent* event)
    {
        input_ = input;
        auto key = static_cast<Qt::Key>(event->key());

        // ignore autorepeat in shortcut mode
        if (state_ == states::shortcut && event->isAutoRepeat()) return;

        if (event->type() == QEvent::KeyPress)
        {
            if (shortcut_input_.is_trigger_key(static_cast<Qt::Key>(event->key())))
            {
                set_state(states::shortcut);
            }

            if (input_.isEmpty() && state_ == states::command)
            {
                reset();
                return;
            }

            // add user input as first suggestion
            suggestions_.clear();
            if (!input_.isEmpty()) suggestions_.add(nxi::text_suggestion{ input_, ":/icon/search", "search" });

            switch(state_)
            {
                case states::command:
                    command_system_.search(input_, [this](nds::node_ptr<const nxi::command> command)
                    {
                        suggestions_.add(std::move(command));
                    });
                    // suggestions_ = history_system_.search(input_);
                    break;

                case states::command_param:
                    command_->add_suggestion(suggestions_);
                    break;

                case states::shortcut:
                    //suggestions_.add(shortcut_input_.update(event));
                    emit event_shortcut_input_update(shortcut_input_.to_string());
                    break;
            }

            if (suggestion_count() > 0) select_suggestion(0);
            emit event_suggestion_update(suggestions_);
        }

        // shortcut mode need release event
        if (event->type() == QEvent::KeyRelease && state_ == states::shortcut)
        {
            //suggestions_.add(shortcut_input_.update(event));
            emit event_shortcut_input_update(shortcut_input_.to_string());
        }
    }

    void command_input::exec()
    {
        nxi_trace(" exec {} - {}", input_, state_text());


        if (state_ == states::command)
        {
            //qDebug() << "exec action" << suggestion(selected_suggestion_index_).text();

            //if (suggestion(selected_suggestion_index_).type() == nxi::suggestion_type::command)            {
                //const nxi::command& command = static_cast<const nxi::basic_suggestion<const nxi::command>&>(suggestion(selected_suggestion_index_)).get();
                //command_ = stz::make_observer(&command);
            //}
            // required parameters
            if (command_ && command_->params().size() > 0)
            {
                reset();
                set_state(states::command_param);
            }
            else
            {
                command_system().exec(command_);
                reset();
            }
        }
        else if (state_ == states::command_param)
        {
            params_.add(input_);
            param_index_++;

            // no more parameters
            if (command_ && param_index_ >= command_->params().size())
            {
                // exec command
                if (command_) command_->exec(params_);

                qDebug() << "final exec " << command_->name() << "  " << input_;
                reset();
            }
        }
    }

    command_input::states command_input::state() const { return state_; }

    QString command_input::state_text() const
    {
        if (state_ == states::command) return "command";
        if (state_ == states::shortcut) return "shortcut";
        if (state_ == states::command_param) return "command_param";
        //if (state_ == states::command_param && param_index_ < command_->params().size()) return command_->params()[param_index_];

        return "no command";
    }

    void command_input::set_state(states state)
    {
        state_ = state;
        if (state == states::command_param)
        {
            auto p = command_->params()[param_index_];
            emit event_command_param_required(p);
        }
    }

    const nxi::suggestion_vector& command_input::suggestions() const
    {
        return suggestions_;
    }

    const QString& command_input::text() const { return input_; }
    size_t command_input::suggestion_count() const { return suggestions_.size(); }
    bool command_input::is_empty() const { return input_.isEmpty(); }

    bool command_input::is_valid() const
    {
        return true;
    }

    const nxi::command_params& command_input::params()
    {
        return params_;
    }

    void command_input::select_suggestion(int index)
    {
        selected_suggestion_index_ = index;

        //set_input(suggestion(selected_suggestion_index_).text());
        emit event_selection_update(index);

        if (command_ && command_->preview())
        {
            nxi::command_params params;
            auto z = input_;
            params.add(input_);
            command_system_.exec(command_, params);
            set_input(z);
        }
    }

    void command_input::select_previous_suggestion()
    {
        if (selected_suggestion_index_ > 0) select_suggestion(selected_suggestion_index_ - 1);
    }

    void command_input::select_next_suggestion()
    {
        if (selected_suggestion_index_ + 1 < suggestions_.size()) select_suggestion(selected_suggestion_index_ + 1);
    }

    const suggestion_vector::suggestion_type& command_input::suggestion(int index)
    {
        nxi_assert(index >= 0 && index < suggestions_.size());
        return suggestions_[index];
    }

    const suggestion_vector::suggestion_type& command_input::selected_suggestion()
    {
        return suggestion(selected_suggestion_index_);
    }

    void command_input::suggest_command()
    {
        suggestions_.clear();
        command_system_.root_list([this](nds::node_ptr<const nxi::command> command)
        {
            suggestions_.add(std::move(command));
        });
        emit event_suggestion_update(suggestions_);
    }

    void command_input::suggest_page()
    {
        suggestions_.clear();
        for (const auto& page :  nxi_core_.page_system().list_root())
        {
            suggestions_.add(page);
        }
        emit event_suggestion_update(suggestions_);
    }

    void command_input::reset()
    {
        param_index_ = 0;
        selected_suggestion_index_ = -1;
        set_state(states::command);
        params_.clear();
        suggestions_.clear();
        emit event_reset();
    }

    int command_input::selected_suggestion_index()
    {
        return selected_suggestion_index_;
    }

    bool command_input::has_selected_suggestion()
    {
        return selected_suggestion_index_ > -1;
    }

    void command_input::clear()
    {
        input_ = "";
        reset();
    }

    nxi::command_system& command_input::command_system()
    {
        return command_system_;
    }

    nxi::shortcut_input& command_input::shortcut_input()
    {
        return shortcut_input_;
    }

    void command_input::set_input(const QString& input)
    {
        input_ = input;
        emit event_input_update(input_);
    }
} // nxi