#ifndef INCLUDE_NXI_VECTOR_HPP_NXI
#define INCLUDE_NXI_VECTOR_HPP_NXI

#include <nxi/suggestion.hpp>

#include <stz/observer_ptr.hpp>
#include <vector>
#include <QObject>
#include <QString>

namespace nxi
{
    class command;
    class page;
    class suggestion;

    class suggestion_vector : public QObject
    {
        Q_OBJECT
    public:
        using suggestion_type = nxi::suggestion;
        using suggestions_type = std::vector<suggestion_type>;

    public:
        suggestion_vector() = default;
        suggestion_vector(const suggestion_vector&) = delete;

        suggestions_type::iterator begin();
        suggestions_type::iterator end();
        suggestions_type::const_iterator begin() const;
        suggestions_type::const_iterator end() const;

        void push_back(QString text);
        void push_back(const char* text);

        template<class T>
        void push_back(T item)
        {
            suggestions_.emplace_back(nxi::suggestion{ std::move(item) });
        }

        void erase(int index);
        void erase(nds::node_ptr<const nxi::page>);

        void select(int index) const;
        void select_previous() const;
        void select_next() const;
        size_t suggestion_count() const;
        const suggestion_type& suggestion(int index) const;
        const suggestion_type& selected() const;
        int selected_index() const;
        bool has_selection() const;

        void clear();
        std::size_t size() const;
        const suggestion_type& operator[](unsigned int index) const;

    signals:
        void event_selection_update(int index) const;
        void event_update(stz::observer_ptr<const nxi::suggestion_vector> suggestions) const;

    private:
        suggestions_type suggestions_;

        mutable int selected_index_;
    };
} // nxi

#endif // INCLUDE_NXI_VECTOR_HPP_NXI
