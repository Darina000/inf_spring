#include <thread>
#include <future>
#include <vector>
#include <iostream>

template<typename Iterator,typename Func>
void parallel_for_each(Iterator first, Iterator last, Func f)
{
    unsigned long const length = std::distance(first, last);
    if (!length){
        return;
    }
    unsigned long const min_per_thread = 25;
    if (length < (2 * min_per_thread)){
        std::for_each(first, last, f);
    }else{
        Iterator const mid_point = first + length / 2;
        std::future<void> first_half = std::async(&parallel_for_each<Iterator, Func>, first, mid_point, f);
        parallel_for_each(mid_point, last, f);
        first_half.get();
    }
}

int main() {
    std::vector<int> nums{3, 4, 2, 8, 15, 267, 23, 42};
    std::for_each(nums.begin(), nums.end(), [](const int& n) { std::cout << " " << n;});
    return 0;
}
