#ifndef INCLUDE_NXI_SUGGESTION_BASIC_HPP_NXI
#define INCLUDE_NXI_SUGGESTION_BASIC_HPP_NXI

#include <nxi/suggestion.hpp>
#include <nxi/command.hpp>

namespace nxi
{
    template<class T>
    struct basic_suggestion : public suggestion
    {
        static inline QString unknown_type = "unknown_suggestion_type";

        basic_suggestion(const T& data)
            : nxi::suggestion(suggestion::get_type<std::decay_t<T>>())
            , data_{ data }
        {}

        const QString& text() const override
        {
            if constexpr (std::is_same_v<T, nxi::command>)
            {
                return data_.name();
            }
            return unknown_type;
        }

        const T& get() const { return data_; }

    private:
        const T& data_;
    };
} // nxi

#endif // INCLUDE_NXI_SUGGESTION_BASIC_HPP_NXI
