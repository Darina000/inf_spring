
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>
#include <functional>

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
        a += std::chrono::duration_cast <std::chrono::microseconds>(end - m_begin).count();
        
        std::cout << "micro " << a << std::endl;
           
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
        a += std::chrono::duration_cast <std::chrono::microseconds>(en - m_begin).count();
        std::cout << "stop  on " << a<< std::endl;
        
    }

    void dur(){
        m_begin = clock_t::now();
        std::cout << "dur from "  << a << std::endl;
    };

    void uninitialize(){};
};

void f(){
    std::vector <int> v;
    v.reserve(10000);
    
    for (auto i=10000; i>=0; --i ){
        v.push_back(i);
    }
}


int main()
{
    Timer t;
    f();
    t.stop();
    f();
    f();
    t.dur();
    for (int i = 0; i < 5; i++){
        f();
    }
    t.stop();
    f();
    t.dur();
    f();
        
    return 0;

}
