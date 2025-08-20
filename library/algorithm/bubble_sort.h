#pragma once
#include "../function.h"

template<typename iterator, auto predicate = library::less<decltype(*std::declval<iterator>())>>
inline void bouble_sort(iterator first, iterator last) {
}