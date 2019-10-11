#include <nxi/command/input.hpp>

#include <nxi/command.hpp>
#include <nxi/system/command.hpp>


namespace nxi
{

    command_input::command_input(nxi::command_system& command_system)
        : command_system_{ command_system }
        , state_{ states::action }
    {}

    void command_input::add_param(const QString& param)
    {

    }

    void command_input::update(const QString& input)
    {
        input_ = input;

        if (state_ == states::action)
        {
            auto suggestion = command_system_.search(input_);
            emit event_suggestion_update(suggestion);
        }
        if (state_ == states::param)
        {
            //command_->suggest_param(param_index_);
            param_suggestions_.clear();
            command_->add_suggestion(param_suggestions_);
        }
    }

    void command_input::exec()
    {
        if (state_ == states::action)
        {
            qDebug() << "exec action";
            auto& command = suggestions_[0];
            // required parameters
            if (command->params().size() > 0)
            {
                state_ = states::param;
                input_ = "";
                param_index_ = 0;
                command_ =  suggestions_[0];
                suggestions_.clear();
            }
            else command->exec();
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
                input_ = "";
                param_index_ = 0;
                state_ = states::action;
                params_.clear();
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
} // nxi