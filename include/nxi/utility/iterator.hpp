#ifndef INCLUDE_NXI_UTILITY_ITERATOR_HPP_NXI
#define INCLUDE_NXI_UTILITY_ITERATOR_HPP_NXI

namespace nxi
{
    template<class Iterator>
    class dereference_iterator : public Iterator
    {
    public:
        using value_type = typename Iterator::value_type::element_type;

        dereference_iterator(const Iterator& other) :
            Iterator(other)
        {}

        const value_type& operator*() const
        {
            return *(Iterator::operator*());
        }

        const value_type* operator->() const
        {
            return Iterator::operator*().get();
        }

        const value_type& operator[](size_t n) const
        {
            return *(Iterator::operator[](n));
        }
    };
} // nxi

#endif // INCLUDE_NXI_UTILITY_ITERATOR_HPP_NXI
