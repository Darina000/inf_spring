#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>
#include <cstdlib>

#include <execution>

class Timer
{
public:
    
    using clock_t = std::chrono::steady_clock;
    using time_point_t = clock_t::time_point;
    
    Timer(): m_begin(clock_t::now()), stay(true){
    }
    
    ~Timer() noexcept
    {
        try
            {
                uninitialize();
            }
            catch (...)
            {
                // std::abort();
            }
        }
private:
    time_point_t m_begin;
    int a = 0;
    bool stay;
    
public:
    void stop(){
        if (stay){
            auto en = clock_t::now();
            a += std::chrono::duration_cast <std::chrono::nanoseconds>(en - m_begin).count();
            stay = false;
            std::cout << "stop on " << a << std::endl;
        }
        
    }

    void dur(){
        if(!stay){
            m_begin = clock_t::now();
            stay = true;
            std::cout << "dur from "  << a << std::endl;
        }
    };

    void uninitialize(){
        if(stay){
            auto end = clock_t::now();
            a += std::chrono::duration_cast <std::chrono::nanoseconds>(end - m_begin).count();
        }
        std::cout << "nanoseconds " << a << std::endl;
    };
};

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
    
    td::cout << "for_each" << std::endl;
        {
            std::vector<int> nums{3, 4, 2, 8, 15, 267, 23, 42};
            Timer t;
            std::for_each(nums.begin(), nums.end(), [](const int& n) { std::cout << " " << n;});
        }
        std::cout << "parallel_for_each" << std::endl;
        {
            std::vector<int> nums{3, 4, 2, 8, 15, 267, 23, 42};
            Timer t;
            parallel_for_each(nums.begin(), nums.end(), [](const int& n) { std::cout << " " << n;});
        }
        
        std::cout << "partial_sum" << std::endl;
        {
            std::vector<int> nums{3, 4, 2, 8, 15, 267, 23, 42};
            Timer t;
            std::partial_sum(nums.begin(), nums.end(), nums.begin(), std::multiplies<int>());
        }
        std::cout << "exclusive_scan" << std::endl;
        {
            std::vector<int> nums{3, 4, 2, 8, 15, 267, 23, 42};
            Timer t;
            std::exclusive_scan(nums.begin(), nums.end(),
                          std::ostream_iterator<int>(std::cout, " "),
                          1, std::multiplies<>{});
        }
        
        std::cout << "transform_reduce" << std::endl;
        
        {
            std::vector<int> a{0, 1, 2, 3, 4};
            std::vector<int> b{5, 4, 2, 3, 1};
            Timer t;
            std::transform_reduce(std::execution::seq, a.begin(), a.end(),b.begin(), 0.0
                );
        }
        std::cout << "inner_product" << std::endl;
        {
            std::vector<int> a{0, 1, 2, 3, 4};
            std::vector<int> b{5, 4, 2, 3, 1};
            Timer t;
            std::inner_product(a.begin(), a.end(), b.begin(), 0);
        }
    
    /*
     for_each
      nanoseconds 35572
     parallel_for_each
      nanoseconds 2111
     
     partial_sum
      nanoseconds 421
     exclusive_scan
      nanoseconds 1998
     
     transform_reduce
      nanoseconds 666
     inner_product
      nanoseconds 274
     */
    return 0;
}
