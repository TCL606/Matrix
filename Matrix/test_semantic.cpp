#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include "TCL_Matrix.h"
#include <cassert>
#include <iostream>
#include <iterator>

#if defined(_MSC_VER)
#   if _MSVC_LANG >= 202002L
#       define HAS_CXX20
#   endif
#else
#   if __cplusplus >= 202002L
#       define HAS_CXX20
#   endif
#endif

// For further use. To get reason, please continue to read.

#if defined(HAS_CXX20)
#   include <concepts>
#endif

using namespace TCL_Matrix;

namespace
{
    template <typename Container>
    class reverse_container
    {  
    private:
        Container& container;

    public:
        reverse_container(Container& container) noexcept : container(container) {}

        auto begin() noexcept(noexcept(this->container.rbegin())) -> decltype(this->container.rbegin())
        {
            return this->container.rbegin();
        }

        auto end() noexcept(noexcept(this->container.rend())) -> decltype(this->container.rend())
        {
            return this->container.rend();
        }

    };
}

int main()
{
    Matrix m
    {
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 }
    };

    auto x1 = m.begin();
    auto x2 = m.cbegin();
    assert(x1 == x2);
    assert((x1 + 2) > x2);

    bool b = x1 == x1;
    (void)b;

    ++x1;
    assert((*x1)[1] == 5);
    --x1;

    (void)(5 + x1);

    x2 += 2;
    assert((*(x2 - 1))[2] == 6);
    x2 -= 2;

    (void)*(*x2).begin();
    *(*x1).begin() = 999;

    for (auto&& row : m)
    {
        row[2] = 888;
    }

    for (auto&& row : m)
    {
        assert(row[2] == 888);
        for (auto&& e : row)
        {
            if (&e != &row[2])
            {
                assert(e != 888);
            }
        }
    }

    m.Display();

    std::cout << std::endl;
    for (auto&& row : reverse_container<Matrix>(m))
    {
        for (const auto& e : reverse_container<std::remove_reference<decltype(row)>::type>(row))
        {
            std::cout << e << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

#if defined(HAS_CXX20)
    static_assert(::std::random_access_iterator<Matrix::iterator>, "Not random access iterator!");
    static_assert(::std::random_access_iterator<Matrix::const_iterator>, "Not random access iterator!");
    static_assert(::std::random_access_iterator<Matrix::reverse_iterator>, "Not random access iterator!");
    static_assert(::std::random_access_iterator<Matrix::const_reverse_iterator>, "Not random access iterator!");
#endif

    return 0;
}
