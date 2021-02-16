#include <iostream>
#include <vector>
#include <deque>
#include <array>
#include <list>
#include <forward_list>
#include <algorithm>


class Timer
{
public:
    
    using clock_t = std::chrono::steady_clock;
    using time_point_t = clock_t::time_point;
    
    Timer(): m_begin(clock_t::now()){}
    
    ~Timer() noexcept
    {
     //  m_bRunning = false;
        
        auto end = clock_t::now();
        a += std::chrono::duration_cast <std::chrono::nanoseconds>(end - m_begin).count();
        
        std::cout << "nanoseconds " << a << std::endl;
           
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
    double a = 0;
    
public:
    void stop(){
        auto en = clock_t::now();
        a += std::chrono::duration_cast <std::chrono::nanoseconds>(en - m_begin).count();
        std::cout << "stop  on " << a<< std::endl;
        
    }

    void dur(){
        m_begin = clock_t::now();
        std::cout << "dur from "  << a << std::endl;
    };

    void uninitialize(){};
};

int main()
{
    {
    std::vector<int> v1 = { 10, 8, 6, 4, 2, 1 };
    Timer t;
    std::sort(begin(v1), end(v1));
    }
    
    {
    std::array<int, 6> v3  = { 10, 8, 6, 4, 2, 1 };
    Timer t;
    std::sort(v3.begin(), v3.end());
    }
    
    {
    std::deque<int> v2 = { 10, 8, 6, 4, 2, 1 };
    Timer t;
    std::sort(begin(v2), end(v2));
    }
    
    {
    std::list<int> v4 = {  10, 8, 6, 4, 2, 1  };
    Timer t;
    v4.sort();
    }
    
    {
    std::forward_list<int> v5 = { 10, 8, 6, 4, 2, 1 };
    Timer t;
    v5.sort();
    }
    
    return  0;
}
