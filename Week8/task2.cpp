#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <string>
#include <functional>
#include <ctime>
#include <mutex>
#include <iterator>


void S_Search(std::mutex &m_mutex, const std::string &string, const std::string &substring, std::vector<size_t>& Iterators, size_t left, size_t right)
{
    std::lock_guard < std::mutex > lock(m_mutex);
    int count = 0;
    for (size_t i = left; i < right; i++)
    {
        for (size_t j = 0; j < substring.size(); j++)
        {
            if (i + j < string.size())
            {
                count += (string[i + j] == substring[j]);
            }
        }
        if (count == substring.size())
        {
            Iterators.push_back(i);
        }
        count = 0;
    }
}


void parallel_search(std::mutex &m_mutex, const std::string &string, const std::string &substring, std::vector<size_t>& Iterators)
{
    const std::size_t num_core = std::thread::hardware_concurrency();
    int hardware_threads = num_core != 0 ? num_core : 8;
    
    int thread_x_size = string.size() / hardware_threads;
    int r = string.size() % hardware_threads;
    std::vector<std::thread> threads(hardware_threads);
    double left = 0, right = thread_x_size;
    
    
    
    for (int i = 0; i < threads.size(); ++i)
    {
        threads[i] = std::thread(S_Search, m_mutex, string, substring, std::ref(Iterators), left, right);
        left += thread_x_size;
        right += thread_x_size;
    }


    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}


int main()
{
    
    std::mutex m_mutex;
    std::random_device rd;
    
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> d(0, 3);
    
    std::vector <char> DNA = { 'A', 'G', 'T', 'C' };
    std::string st = "";

    int n;
    for (int i = 0; i < 1000; ++i) {
        st += DNA[d(gen)];
    }
    std::cout << st << std::endl;
    
    
    std::string s_find;
    std::cout << "Find: ";
    std::cin >> s_find;
    
    
    std::vector<size_t> Iterators;
    parallel_search(m_mutex, st, s_find, Iterators);

    for (int i = 0; i < Iterators.size(); ++i) {
        std::cout << "On line: " << Iterators[i] << std::endl;
    }

    if (Iterators.empty()) {
        std::cout << "Not found" << std::endl;
    }

    return 0;
}


