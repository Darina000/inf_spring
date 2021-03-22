#include <iostream>
#include <vector>
#include <random>
#include <future>

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
    double pi = ((double) tot_points_inside/(double) tot_samples)*4.0;
    return pi;
}


double m_pi_paral(long tot_samples, int n_threads){
    long samples_per_thread = tot_samples / n_threads;
    std::vector<std::future<long>> futures;
    for(int t = 0; t < n_threads; ++t){
        futures.emplace_back(async(std::launch::async, random_circle_sampling, samples_per_thread));
    }

    long tot_points_inside = 0;
    for(std::future<long>& f : futures){
        tot_points_inside += f.get();
    }

    double pi = 4.0 * (double) tot_points_inside / (double) tot_samples;
    return pi;

}



int main() {
    long tot_samples = 1e6;
    int n_threads = 8;
    double pi = m_pi(tot_samples);
    double pi_p = m_pi_paral(tot_samples,n_threads);
    std::cout << "pi " << pi << std::endl;
    std::cout << "pi_p " << pi_p << std::endl;
    return 0;
}
