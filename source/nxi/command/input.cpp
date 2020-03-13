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
        , mode_{ mode::input }
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
        if (mode_ == mode::shortcut && event->isAutoRepeat()) return;

        if (event->type() == QEvent::KeyPress)
        {
            suggestions_.clear();

            if (mode_ != mode::shortcut && shortcut_input_.is_trigger_key(static_cast<Qt::Key>(event->key())))
            {
                set_mode(mode::shortcut);
            }

            if (mode_ == mode::shortcut)
            {
                shortcut_input_.update(event, suggestions_);
                emit event_shortcut_input_update(shortcut_input_.to_string());
            }
            if (mode_ == mode::input)
            {
                // add user input as first suggestion
                if (!input_.isEmpty())
                {
                    suggestions_.push_back(nxi::search_suggestion{ input_, "Google", "https://www.google.com/search?q=", ":/icon/search" } );
                    suggestions_.push_back(nxi::search_suggestion{ input_, "CppReference", "https://en.cppreference.com/mwiki/index.php?search=", ":/icon/search" } );
                }

                command_system_.search(input_, [this](nds::node_ptr<const nxi::command> command)
                {
                    suggestions_.push_back(std::move(command));
                });
            }

            if (suggestion_count() > 0) select_suggestion(0);
            emit event_suggestion_update(suggestions_);

            /*
            switch (nxi_core_.context().active())
            {
                case nxi::context::command:
                    suggestion_.add(cmd);
                    break;
                case nxi::context::page:
                    suggestion_.add(cmd);
                    break;
            }*/
        }

        // shortcut mode need release event
        if (mode_ == mode::shortcut && event->type() == QEvent::KeyRelease)
        {
            shortcut_input_.update(event, suggestions_);
            if (!shortcut_input_.is_triggered()) set_mode(mode::input);
            emit event_shortcut_input_update(shortcut_input_.to_string());
            emit event_suggestion_update(suggestions_);
        }
    }

    void command_input::exec()
    {
        nxi_assert(has_selected_suggestion());
        nxi_trace(" exec {} - {}", input_, state_text());

        selected_suggestion().apply([this](auto&& suggestion)
        {
            using suggestion_type = std::decay_t<decltype(suggestion)>;
            if constexpr (std::is_same_v<suggestion_type, nds::node_ptr<const nxi::command>>)
            {
                nxi_core_.command_system().exec(suggestion);
            }
            else if constexpr (std::is_same_v<suggestion_type, nds::node_ptr<const nxi::page>>)
            {
                //nxi_core_.page_system().load(suggestion);
            }
            else if constexpr (std::is_same_v<suggestion_type, nxi::search_suggestion>)
            {
                nxi_core_.page_system().open<nxi::web_page>(0, suggestion.url() + suggestion.text());
                //nxi_core_.search(suggestion.text());
            }
            else nxi_warning("unknown suggestion");
        });

        reset();

        /*
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
         */
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

    void command_input::set_mode(command_input::mode mode)
    {
        mode_ = mode;
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
            suggestions_.push_back(std::move(command));
        });
        emit event_suggestion_update(suggestions_);
    }

    void command_input::suggest_page()
    {
        suggestions_.clear();
        for (const auto& page :  nxi_core_.page_system().list_root())
        {
            suggestions_.push_back(page);
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
        set_mode(mode::input);
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

    const nxi::command_system& command_input::command_system() const
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