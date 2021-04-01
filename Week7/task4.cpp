#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>
#include <functional>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <cerrno>
#include <cfenv>
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


template <class T> struct func{
  T operator() (const T& x, const T& y) const {
      return x*y/(std::exp(std::cosh(std::tan(0.3)*23456789 + std::cos(0.4*87697)))*(std::sin(M_PI*0.435)/(12.0)*(std::tan(std::cos(0.987*0.887697)))));
  }
};

int main() {
    
    std::cout << "for_each" << std::endl;
        {
            std::vector < double > nums(400007, 0.5);
                        Timer t;
                        parallel_for_each(nums.begin(), nums.end(), [](double& n) {
                            for (int i = 0; i< 40; ++i){
                                n +=  std::exp(std::cosh(std::tan(0.3*i)*23456789 + std::cos(0.4*i*87697)))*(std::sin(i*M_PI)/(12.0)*(std::tan(i + std::cos(0.987*i*87697))));
                             }
                            
                        });
        }
    
        std::cout << "parallel_for_each" << std::endl;
        {
            std::vector < double > nums(400007, 0.5);
            Timer t;
            
            std::for_each(std::execution::par, nums.begin(), nums.end(), [](double& n) {
                for (int i = 0; i< 40; ++i){
                    n +=  std::exp(std::cosh(std::tan(0.3*i)*23456789 + std::cos(0.4*i*87697)))*(std::sin(i*M_PI)/(12.0)*(std::tan(i + std::cos(0.987*i*87697))));
                
                 }
            });
        
        }
        
        std::cout << "partial_sum" << std::endl;
        {
            std::vector < double > nums(10'000'007, 0.5);
            std::vector < double > nums2;
            Timer t;
            std::partial_sum(nums.begin(), nums.end(), nums.begin(), func<double>());
        }
        std::cout << "inclusive_scan" << std::endl;
        {
            std::vector < double > nums(10'000'007, 0.5);
            std::vector < double > nums2;
            Timer t;
            std::inclusive_scan(std::execution::par, nums.begin(), nums.end(), nums.begin(),  func<double>());
        }
        
        std::cout << "transform_reduce" << std::endl;
           
           {
               std::vector < double > a(10'000'007, 0.1);
               std::vector < double > b(10'000'007, 0.1);
               Timer t;
               std::transform_reduce(std::execution::par, a.begin(), a.end(), b.begin(),0.0, func<double>(), func<double>());
          //     std::transform_reduce(std::execution::seq, a.begin(), a.end(), b.begin(), 0.0, func<double>(), func<double>());
           }
           std::cout << "inner_product" << std::endl;
           {
               std::vector < double > a(10'000'007, 0.1);
               std::vector < double > b(10'000'007, 0.2);
               Timer t;
               std::inner_product(a.begin(), a.end(), b.begin(), 0, func<double>(), func<double>());
           }
    
    /*
     for_each
     nanoseconds 190522800
     parallel_for_each
     nanoseconds -129359563
     partial_sum
     nanoseconds 388431643
     inclusive_scan
     nanoseconds 900924615
     transform_reduce
     nanoseconds 637388938
     inner_product
     nanoseconds 668797358
     */
    return 0;
}
