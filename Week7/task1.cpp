#include <iostream>
#include <vector>
#include <random>
#include <future>

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


long random_circle_sampling(long n_samples){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    long points_inside = 0;
    for(long i = 0; i < n_samples; ++i){
        double x = dis(gen);
        double y = dis(gen);
        if(x*x + y*y <= 1.0){
            ++points_inside;
        }
    }
    return points_inside;
}

double m_pi(long tot_samples){
    double tot_points_inside = random_circle_sampling(tot_samples);
 //   double pi = ((double) tot_points_inside/(double) tot_samples)*4.0;
    return ( tot_points_inside/ tot_samples)*4.0;
}


double m_pi_paral(long tot_samples){
    
    const std::size_t length = tot_samples;

    if (!length)
        return tot_samples;
    
    
    const std::size_t min_per_thread = 25;
    const std::size_t max_threads =
        (length + min_per_thread - 1) / min_per_thread;

    const std::size_t hardware_threads =
        std::thread::hardware_concurrency();

    const std::size_t n_threads =
        std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    
    
    long samples_per_thread = tot_samples / n_threads;
    std::vector<std::future<long>> futures;
    for(int t = 0; t < n_threads; ++t){
        futures.emplace_back(async(std::launch::async, random_circle_sampling, samples_per_thread));
    }

    long tot_points_inside = 0;
    for(auto& f : futures){
        tot_points_inside += f.get();
    }

   // double pi = 4.0 * (double) tot_points_inside / (double) tot_samples;
    return ((double) tot_points_inside / (double) tot_samples)*4.0;

}



int main() {
    long tot_samples = 1e6;
    {
        Timer t;
        double pi = m_pi(tot_samples);  // nanoseconds 175763493
        
    }
    {
        Timer t;
        double pi_p = m_pi_paral(tot_samples); // nanoseconds 80224339   быстрее
    }
    
   // std::cout << "pi " << pi << std::endl;
   // std::cout << "pi_p " << pi_p << std::endl;
    return 0;
}

