#ifndef INCLUDE_NXI_COMMAND_INPUT_HPP_NXI
#define INCLUDE_NXI_COMMAND_INPUT_HPP_NXI

#include <nxi/command.hpp>
#include <stz/observer_ptr.hpp>
#include <vector>

#include <QString>
#include <include/nxi/system/page.hpp>

namespace nxi
{
    class command_system;

    class command_input : public QObject
    {
        Q_OBJECT
    public:
        enum class states { action, param };

        command_input(nxi::command_system& command_system);

        void add_param(const QString& param);

        void update(const QString& input);
        void exec();

        states state() const;
        QString state_text() const;

        const nxi::command_params& params();
        std::vector<stz::observer_ptr<nxi::command>> suggestions() const;
        const std::vector<QString>& param_suggestions() const;
        const QString& text() const;
        size_t suggestion_count() const;
        bool is_empty() const;
        bool is_valid() const;

    signals:
        void event_suggestion_update(std::vector<stz::observer_ptr<nxi::command>>);
        //void event_suggestion_update(std::vector<stz::observer_ptr<nxi::page>>);

    private:
        QString input_;
        states state_;
        nxi::command_params params_;
        stz::observer_ptr<nxi::command> command_;
        int param_index_ = 0;
        nxi::command_system& command_system_;
        std::vector<stz::observer_ptr<nxi::command>> suggestions_;
        std::vector<QString> param_suggestions_;
    };
} // nxi

#endif // INCLUDE_NXI_COMMAND_INPUT_HPP_NXI
