#pragma once

#include <stdexcept>

/*
 0, 1, 2,  3,  4,  5,   6, 7, 8
[3, 8, 22, 1,  17, 383, 0, 2, 5]
[1, 3, 8,  22, 
               0,  2,   5, 17, 383]
[0, 1, 2, 3, 5, 8, 17, 22, 383]


(0 + 9)/2 = 4
(0;4-0), (4:9-4)


(0 + 4)/2 = 2
(4 + 5)/2 = 5
*/
template<typename T> void mgsrt_part(std::vector<T>&v, size_t off, size_t count){
    if(count == 1)
        return;

    size_t second_off = (off + count)/2;
    size_t first_count = second_off - off;
    size_t second_count = count - first_count;

    mgsrt_part(v, off, first_count);
    mgsrt_part(v, second_off, second_count);

    std::vector<T> merged(count, T{});
    for()
}

template <typename T> void mgsrt(std::vector<T> &v) {
    mgsrt_part
}
