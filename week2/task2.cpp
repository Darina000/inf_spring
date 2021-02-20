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


int main()
{
    {
        
    std::vector<int> v1;
        for (int i = 0; i<10000; ++i ){
            v1.push_back(10000-i);
        }
    Timer t;
    std::sort(begin(v1), end(v1)); //23485 nanoseconds
    }
    
    {
    std::array<int, 10000> v3;
        for (int i = 0; i<10000; ++i ){
            v3[i] += (10000-i);
        }
    Timer t;
    std::sort(v3.begin(), v3.end()); //473519
        
    }
    
    {
    std::deque<int> v2;
        for (int i = 0; i<10000; ++i ){
            v2.push_back(10000-i);
        }
    Timer t;
    std::sort(begin(v2), end(v2));  //641095
    }
    
    {
    std::list<int> v4;
        for (int i = 0; i<10000; ++i ){
            v4.push_back(10000-i);
        }
    Timer t;
    v4.sort(); //3137970
    }
    
    {
    std::forward_list<int> v5;
        for (int i = 0; i<10000; ++i ){
            v5.push_front(10000-i);
        }
    Timer t;
    v5.sort(); //925533
    }
    
    // топ: vector, array, deque, forward_list, list
    return  0;
}
