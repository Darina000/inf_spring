#include <iostream>
#include <vector>
#include <deque>
#include <array>
#include <list>
#include <forward_list>
#include <algorithm>
#include <set>
#include <cstdlib>

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
    int N;
    std::cout << "N: ";
    std::cin >> N;
    
    std::vector <int> c;
    
    for(int i = 0; i<N; ++i){
        c.push_back(i);
    }
    {
        std::set<int> b;
        Timer time;
        for(int i = 0; i<N; ++i){
            b.insert(c[i]);
        }
    }
    {
        std::vector<int> t;
        Timer time1;
        for(int i = 0; i<N; ++i){
            t.push_back(c[i]);
        }
        std::sort(begin(t), end(t),  std::greater<int>());
    }
    
    
    // N = 100: set: 243571, vector: 44638
    // N = 10000: set: 13498113, vector: 1340106
    // N = 100000: set: 116206375, vector: 9157211
    return  0;
}
