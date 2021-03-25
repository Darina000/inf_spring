#include <algorithm>
#include <future>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

#include <algorithm>
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





class Threads_Guard
{
public:

    explicit Threads_Guard(std::vector < std::thread > & threads) :
        m_threads(threads)
    {}

    Threads_Guard            (Threads_Guard const&) = delete;

    Threads_Guard& operator=(Threads_Guard const&) = delete;

    ~Threads_Guard() noexcept
    {
        try
        {
            for (std::size_t i = 0; i < m_threads.size(); ++i)
            {
                if (m_threads[i].joinable())
                {
                    m_threads[i].join();
                }
            }
        }
        catch (...)
        {
            // std::abort();
        }
    }

private:

    std::vector < std::thread > & m_threads;
};

template < typename Iterator, typename T >
struct accumulate_block
{
    T operator()(Iterator first, Iterator last)
    {
        return std::accumulate(first, last, T());
    }
};

template < typename Iterator, typename T, typename T2 >
T parallel_accumulate(Iterator first, Iterator last, T init, T2 num_threads)
{
    const std::size_t length = std::distance(first, last);

    if (!length)
        return init;

 //   const std::size_t min_per_thread = 25;
 //   const std::size_t max_threads =
 //       (length + min_per_thread - 1) / min_per_thread;

 //   const std::size_t hardware_threads =
 //       std::thread::hardware_concurrency();

    //const std::size_t num_threads =
    //    std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);

    const std::size_t block_size = length / num_threads;

    std::vector < std::future < T > > futures(num_threads - 1);
    std::vector < std::thread >          threads(num_threads - 1);

    Threads_Guard guard(threads);

    Iterator block_start = first;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);

        std::packaged_task < T(Iterator, Iterator) > task{
            accumulate_block < Iterator, T >() };

        futures[i] = task.get_future();
        threads[i] = std::thread(std::move(task), block_start, block_end);

        block_start = block_end;
    }

    T last_result = accumulate_block < Iterator, T > ()(block_start, last);

    T result = init;

    for (std::size_t i = 0; i < (num_threads - 1); ++i)
    {
        result += futures[i].get();
    }

    result += last_result;

    return result;
}

int main()
{
    std::vector < int > v(100);

    std::iota(v.begin(), v.end(), 1);
    
    std::size_t number;
    std::cin >> number;
    {
        Timer time;
        parallel_accumulate(v.begin(), v.end(), 0, number);
    }
    return 0;
}
