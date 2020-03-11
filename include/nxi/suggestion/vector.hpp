#ifndef INCLUDE_NXI_SUGGESTION_VECTOR_HPP_NXI
#define INCLUDE_NXI_SUGGESTION_VECTOR_HPP_NXI

#include <nxi/suggestion.hpp>

#include <QString>

#include <vector>

namespace nxi
{
    class command;
    class page;
    class suggestion;

    class suggestion_vector
    {
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

        void add(QString text);
        void add(const char* text);

        template<class T>
        void add(T item)
        {
            suggestions_.emplace_back(nxi::suggestion{ std::move(item) });
        }

        void clear();
        std::size_t size() const;
        const suggestion_type& operator[](unsigned int index) const;

    private:
        suggestions_type suggestions_;
    };
} // nxi

#endif // INCLUDE_NXI_SUGGESTION_VECTOR_HPP_NXI
