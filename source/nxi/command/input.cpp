#include <nxi/command/input.hpp>

#include <nxi/command.hpp>
#include <nxi/log.hpp>
#include <nxi/system/command.hpp>

#include <QKeyEvent>

namespace nxi
{

    command_input::command_input(nxi::command_system& command_system)
        : command_system_{ command_system }
        , state_{ states::action }
        , selected_suggestion_index_{ -1 }
    {}

    void command_input::add_param(const QString& param)
    {

    }

    void command_input::update(const QString& input, QKeyEvent* event)
    {
        if (event->isAutoRepeat()) return;

        input_ = input;

        /*
        if (input_.isEmpty())
        {
            emit event_complete();
            return;
        }*/

        if (state_ == states::action)
        {
            suggestions_ = shortcut_input_.update(command_system_, event);
            /*
            suggestions_ = command_system_.search(input_);
            auto shortcut_suggestions = shortcut_input_.update(command_system_, event);
            suggestions_.insert(suggestions_.end(), shortcut_suggestions.begin(), shortcut_suggestions.end());*/

            emit event_suggestion_update(stz::make_observer<nxi::commands_view>(&suggestions_));
        }
        if (state_ == states::param)
        {
            //command_->suggest_param(param_index_);
            param_suggestions_.clear();
            command_->add_suggestion(param_suggestions_);
        }

        select_suggestion(0);
    }

    void command_input::exec()
    {
        nxi_trace(" exec {}", input_);
        if (suggestion_count() == 0) return;

        if (state_ == states::action)
        {
            qDebug() << "exec action" << suggestion(selected_suggestion_index_)->name();
            auto& command = suggestion(0);
            // required parameters
            if (command->params().size() > 0)
            {
                reset();
                state_ = states::param;
                command_ =  suggestion(0);
                qDebug() << "wait params ";
            }
            else
            {
                command->exec();
                reset();
                emit event_complete();
            }
        }
        else if (state_ == states::param)
        {
            qDebug() << "exec param " << command_->params()[param_index_];
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
        if (command_)
        {
            if (state_ == states::action) return command_->name();
            if (state_ == states::param && param_index_ < command_->params().size()) return command_->params()[param_index_];
        }
        return "no command";
    }


    nxi::command_system::commands_view command_input::suggestions() const
    {
        return suggestions_;
    }
    const std::vector<QString>& command_input::param_suggestions() const
    {
        return param_suggestions_;
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
        emit event_selection_update(index);
    }

    void command_input::select_previous_suggestion()
    {
        if (selected_suggestion_index_ > 0) select_suggestion(selected_suggestion_index_ - 1);
    }

    void command_input::select_next_suggestion()
    {
        if (selected_suggestion_index_ + 1 < suggestions_.size()) select_suggestion(selected_suggestion_index_ + 1);
    }

    stz::observer_ptr<nxi::command> command_input::suggestion(int index)
    {
        nxi_assert(index >= 0 && index < suggestions_.size());
        return suggestions_[index];
    }

    stz::observer_ptr<nxi::command> command_input::selected_suggestion()
    {
        return suggestion(selected_suggestion_index_);
    }

    void command_input::suggest_command()
    {
        suggestions_ = command_system_.root_list();
        emit event_suggestion_update(stz::make_observer(&suggestions_));
    }

    void command_input::reset()
    {
        param_index_ = 0;
        selected_suggestion_index_ = -1;
        state_ = states::action;
        params_.clear();
        suggestions_.clear();
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

    nxi::shortcut_input& command_input::shortcut_input()
    {
        return shortcut_input_;
    }
} // nxi