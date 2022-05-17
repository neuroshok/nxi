//
// Created by Lezenn on 15/05/2022.
//

#ifndef INCLUDE_NXI_ACTION_VECTOR_HPP_NXI
#define INCLUDE_NXI_ACTION_VECTOR_HPP_NXI

#include <nxi/action.hpp>
#include <nxi/action/page.hpp>
#include <nxi/command.hpp>
#include <nxi/page.hpp>

#include <stz/observer_ptr.hpp>
#include <vector>
#include <QObject>

namespace nxi
{
    class command;
    class page;
    class action;

    /**
     * Action vector
     */
    class action_vector : public QObject
    {
        Q_OBJECT

    public:
        using actions_type = std::vector<action>;

        action_vector() = default;
        action_vector(const action_vector&) = delete;
        action_vector& operator=(const action_vector&) = delete;

        actions_type::iterator begin();
        actions_type::iterator end();

        actions_type::const_iterator begin() const;
        actions_type::const_iterator end() const;

        void push_back(QString text);
        void push_back(const char* text);

        template<class T>
        void push_back(T item)
        {
            actions_.emplace_back(std::move(item));
        }

        void erase(int index);
        void erase(nxi::page_action action);

        void select(int index) const;
        void select_previous() const;
        void select_next() const;

        size_t action_count() const;
        const nxi::action& action(int index) const;
        const nxi::action& selected() const;

        int selected_index() const;
        bool has_selection() const;

        void clear();
        std::size_t size() const;
        const nxi::action& operator[](unsigned int index) const;

    signals:
        void event_selection_update(int index) const;
        void event_update(stz::observer_ptr<const nxi::action_vector> actions) const;

    private:
        std::vector<nxi::action> actions_;
        mutable int selected_index;
    };
}

#endif // INCLUDE_NXI_ACTION_VECTOR_HPP_NXI
