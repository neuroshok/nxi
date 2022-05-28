#include <nxi/command/shortcut_input.hpp>

#include <nxi/command.hpp>
#include <nxi/command/input.hpp>
#include <nxi/suggestion/vector.hpp>
#include <nxi/system/command.hpp>

#include <nxi/log.hpp>
#include <optional>
#include <sstream>

namespace nxi
{
    shortcut_input::shortcut_input(const nxi::command_input& command_input)
        : command_input_{ command_input }
        , is_triggered_{ false }
        , trigger_mode_{ trigger_mode::combo }
    {}

    void shortcut_input::update(QKeyEvent* event, nxi::suggestion_vector& suggestions)
    {
        Qt::Key key = static_cast<Qt::Key>(event->key());

        if (event->type() == QEvent::KeyRelease)
        {
            auto ck = input_.combo_keys.size();
            input_.combo_keys.erase( std::remove( input_.combo_keys.begin(), input_.combo_keys.end(), key ), input_.combo_keys.end() );

            if (input_.combo_keys.size() < ck && trigger_mode_ == trigger_mode::sequence)
            {
                suggestions.clear();
                reset();
                return;
            }

            if (input_.combo_keys.empty() && input_.sequence_keys.empty())
            {
                suggestions.clear();
                reset();
                return;
            }

            // search with new combos keys
            if (trigger_mode_ == trigger_mode::combo)
            {
                suggestions.clear();
                // shift the keys
                key = input_.combo_keys.back();
                input_.combo_keys.pop_back();
                search(key, suggestions);
            }
        }

        if (event->type() == QEvent::KeyPress)
        {
            is_triggered_ = true;
            search(key, suggestions);
        }

        return;
    }

    void shortcut_input::reset()
    {
        is_triggered_ = false;
        input_.combo_keys.clear();
        input_.sequence_keys.clear();
        trigger_mode_ = trigger_mode::combo;
    }

    void shortcut_input::search(Qt::Key key, nxi::suggestion_vector& suggestions)
    {
        nds::node_ptr<nxi::command> found_command;
        bool combo_match = false;
        bool sequence_match = false;

        command_input_.command_system().for_each([&, this](auto&& node)
        {
            const nxi::shortcut& shortcut = node->shortcut();

            // shortcut has combo keys
            if (trigger_mode_ == trigger_mode::combo && shortcut.combo_keys.size() > 0)
            {
                // check previous input combo keys without the new key

                bool combo_partial_match = input_.combo_keys.size() <= shortcut.combo_keys.size() && std::equal(input_.combo_keys.begin(), input_.combo_keys.end(), shortcut.combo_keys.begin());

                if (combo_partial_match)
                {
                    auto next = input_.combo_keys.size();

                    // combo + key match
                    if (next < shortcut.combo_keys.size() && key == shortcut.combo_keys[next])
                    {
                        suggestions.push_back(node);
                        combo_match = true;
                    }
                }
            }

            // shortcut has sequence keys
            if (shortcut.sequence_keys.size() > 0)
            {
                // check previous input combo keys without the new key
                bool sequence_partial_match = input_.sequence_keys.size() <= shortcut.sequence_keys.size() && std::equal(input_.sequence_keys.begin(), input_.sequence_keys.end(), shortcut.sequence_keys.begin());

                if (sequence_partial_match)
                {
                    auto next = input_.sequence_keys.size();

                    // sequence + key match
                    if (next < shortcut.sequence_keys.size() && key == shortcut.sequence_keys[next])
                    {
                        suggestions.push_back(node);
                        sequence_match = true;

                        // combo + sequence match
                        bool combo_fullmatch = std::equal(input_.combo_keys.begin(), input_.combo_keys.end(), shortcut.combo_keys.begin(), shortcut.combo_keys.end());
                        if (combo_fullmatch && (input_.sequence_keys.size() + 1 == shortcut.sequence_keys.size()))
                        {
                            found_command = node;
                        }
                    }
                }
            }
        }); // for_each

        // combo has priority
        if (combo_match) sequence_match = false;

        // switch mode if no combo match
        if (sequence_match) trigger_mode_ = trigger_mode::sequence;

        if (trigger_mode_ == trigger_mode::combo) input_.combo_keys.push_back(key);
        if (trigger_mode_ == trigger_mode::sequence) input_.sequence_keys.push_back(key);



        // command found
        if (found_command)
        {
            nxi_trace("execute shortcut {}", found_command->name());
            reset();
            command_input_.command_system().exec(std::move(found_command));
            return;
        }

        // no match reset input
        if (!combo_match && !sequence_match)
        {
            nxi_trace("unknown sequence {}", input_.to_string());
            reset();
        }
    }

    QString shortcut_input::to_string() const
    {
        return input_.to_string();
    }

    void shortcut_input::add_trigger_key(Qt::Key key)
    {
        trigger_keys_.insert(key);
    }

    bool shortcut_input::is_trigger_key(Qt::Key key) const
    {
        return std::find(trigger_keys_.begin(), trigger_keys_.end(), key) != trigger_keys_.end();
    }

    bool shortcut_input::is_triggered() const
    {
        return is_triggered_;
    }
} // nxi