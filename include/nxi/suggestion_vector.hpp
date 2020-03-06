#ifndef INCLUDE_NXI_SUGGESTION_VECTOR_HPP_NXI
#define INCLUDE_NXI_SUGGESTION_VECTOR_HPP_NXI

#include <nxi/suggestion.hpp>
#include <nxi/suggestion/basic.hpp>
#include <nxi/utility/iterator.hpp>

#include <vector>

namespace nxi
{
    class suggestion_vector
    {
        using suggestions_type = std::vector<std::unique_ptr<const nxi::suggestion>>;
    public:
        suggestion_vector() = default;
        suggestion_vector(const suggestion_vector&) = delete;

        dereference_iterator<suggestions_type::iterator> begin();
        dereference_iterator<suggestions_type::iterator> end();
        dereference_iterator<suggestions_type::const_iterator> begin() const;
        dereference_iterator<suggestions_type::const_iterator> end() const;

        void add(QString text)
        {
            suggestions_.emplace_back(std::make_unique<nxi::suggestion>(nxi::suggestion{ std::move(text) }));
        }

        void add(const char* text)
        {
            add(QString{ text });
        }

        void add(nxi::suggestion suggestion)
        {
            suggestions_.emplace_back(std::make_unique<nxi::suggestion>(std::move(suggestion)));
        }

        template<class T>
        void add(const T& item)
        {
            if constexpr (std::is_same_v<T, nxi::command>)
            {
                suggestions_.emplace_back(std::make_unique<nxi::basic_suggestion<T>>(item));
            }
            else suggestions_.emplace_back(nxi::suggestion{ item });
        }

        void clear();
        std::size_t size() const;
        const nxi::suggestion& operator[](unsigned int index) const;

    private:
        suggestions_type suggestions_;
    };
} // nxi

#endif // INCLUDE_NXI_SUGGESTION_VECTOR_HPP_NXI
