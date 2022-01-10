#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#   define _CRT_SECURE_NO_WARNINGS
#endif

#include "TCL_Matrix.h"
#include <cassert>
#include <iostream>
#include <iterator>

using namespace TCL_Matrix;

#if defined(_MSC_VER)
#   if _MSVC_LANG >= 202002L
#       define HAS_CXX20
#   endif
#else
#   if __cplusplus >= 202002L
#       define HAS_CXX20
#   endif
#endif

#if defined(HAS_CXX20)
#   include <concepts>
#endif

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

    ++x1;
    assert((*x1)[1] == 5);
    --x1;

    x2 += 2;
    assert((*(x2 - 1))[2] == 6);
    x2 -= 2;

    (void)*(*x2).begin();
    *(*x1).begin() = 999;

    for (auto&& row : m)
    {
        row[2] = 888;
    }

    for (const auto&& row : m)
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

#if defined(HAS_CXX20)
    static_assert(::std::random_access_iterator<decltype(m.begin())>, "Not random access iterator!");
    static_assert(::std::random_access_iterator<decltype(m.cbegin())>, "Not random access iterator!");
#endif
    return 0;
}
