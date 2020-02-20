#include <nxi/command/shortcut_input.hpp>

#include <nxi/system/command.hpp>

#include <sstream>
#include <optional>

/*
 * !NEW
 * CTRL ALT + S
 * CTRL + W S
 *
 * search
 */

namespace nxi
{
    shortcut_input::shortcut_input()
    {
        untrigger();
    }

    nxi::commands_view shortcut_input::update(nxi::command_system& command_system, QKeyEvent* event)
    {
        if (event->isAutoRepeat()) return suggestions_;

        Qt::Key key = static_cast<Qt::Key>(event->key());

        if (event->type() == QEvent::KeyRelease)
        {
            auto ck = input_.combo_keys.size();
            input_.combo_keys.erase( std::remove( input_.combo_keys.begin(), input_.combo_keys.end(), key ), input_.combo_keys.end() );

            if (input_.combo_keys.size() < ck && trigger_mode_ == trigger_mode::sequence)
            {
                untrigger();
                return suggestions_;
            }

            if (input_.combo_keys.empty() && input_.sequence_keys.empty())
            {
                untrigger();
                return suggestions_;
            }

            // search with new combos keys
            if (trigger_mode_ == trigger_mode::combo)
            {
                // shift the keys
                key = input_.combo_keys.back();
                input_.combo_keys.pop_back();
                search(command_system, key);
            }
        }

        if (event->type() == QEvent::KeyPress)
        {
            search(command_system, key);
        }

        /*
        qDebug() << "___COMBO";
        for (auto k : input_.combo_keys) qDebug() << k;

        qDebug() << "___SEQ";
        for (auto k : input_.sequence_keys) qDebug() << k;
*/

        return suggestions_;
    }

    void shortcut_input::untrigger()
    {
        suggestions_.clear();
        input_.combo_keys.clear();
        input_.sequence_keys.clear();
        trigger_mode_ = trigger_mode::combo;
    }

    void shortcut_input::search(nxi::command_system& command_system, Qt::Key key)
    {
        suggestions_.clear();

        nxi::command* found_command = nullptr;
        bool combo_match = false;
        bool sequence_match = false;

        command_system.for_each([this, &key, &found_command, &combo_match, &sequence_match](auto&& node)
        {
            const nxi::shortcut& shortcut = node->get().shortcut();

            // shortcut has combo keys
            if (trigger_mode_ == trigger_mode::combo && shortcut.combo_keys.size() > 0)
            {
                // check previous input combo keys without the new key
                bool combo_partial_match = std::equal(input_.combo_keys.begin(), input_.combo_keys.end(), shortcut.combo_keys.begin());

                if (combo_partial_match)
                {
                    auto next = input_.combo_keys.size();

                    // combo + key match
                    if (next < shortcut.combo_keys.size() && key == shortcut.combo_keys[next])
                    {
                        suggestions_.push_back(stz::observer_ptr(&node->get()));
                        combo_match = true;
                    }
                }
            }

            // shortcut has sequence keys
            if (shortcut.sequence_keys.size() > 0)
            {
                // check previous input combo keys without the new key
                bool sequence_partial_match = std::equal(input_.sequence_keys.begin(), input_.sequence_keys.end(), shortcut.sequence_keys.begin());

                if (sequence_partial_match)
                {
                    auto next = input_.sequence_keys.size();

                    // sequence + key match
                    if (next < shortcut.sequence_keys.size() && key == shortcut.sequence_keys[next])
                    {
                        suggestions_.push_back(stz::observer_ptr(&node->get()));
                        sequence_match = true;

                        if (input_.sequence_keys.size() + 1 == shortcut.sequence_keys.size())
                        {
                            found_command = std::addressof(node->get());
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
            qDebug() << "EXEC " << found_command->name();
            found_command->exec();
            untrigger();
        }

        // no match reset input
        if (!combo_match && ! sequence_match)
        {
            qDebug() << "unknown sequence " << input_.combo_keys << " " << input_.sequence_keys;
            untrigger();
        }
    }

    QString shortcut_input::to_string()
    {
        return input_.to_string();
    }

} // nxi