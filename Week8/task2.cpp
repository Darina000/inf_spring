
#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <string>
#include <functional>
#include <ctime>
#include <mutex>
#include <iterator>

std::mutex m_mutex;

int equal(char one, char two)
{
    if (one == two)
    {
        return 1;
    }
    else{
        return 0;
    }
}

void S_Search(std::string string, std::string substring, std::vector<size_t>& Iterators, size_t left, size_t right)
{
    m_mutex.lock();
    int count = 0;
    for (size_t i = left; i < right; i++)
    {
        for (size_t j = 0; j < substring.size(); j++)
        {
            if (i + j < string.size())
            {
                count += equal(string[i + j], substring[j]);
            }
        }
        if (count == substring.size())
        {
            Iterators.push_back(i);
        }
        count = 0;
    }
    m_mutex.unlock();
}


void parallel_search(std::string string, std::string substring, std::vector<size_t>& Iterators)
{
    const std::size_t num_core = std::thread::hardware_concurrency();
    int hardware_threads = num_core != 0 ? num_core : 8;
    
    int thread_x_size = string.size() / hardware_threads;
    int r = string.size() % hardware_threads;
    std::vector<std::thread> threads(hardware_threads);
    double left = 0, right = thread_x_size;
    
    
    
    for (int i = 0; i < threads.size(); ++i)
    {
        threads[i] = std::thread(S_Search, string, substring, std::ref(Iterators), left, right);
        left += thread_x_size;
        right += thread_x_size;
    }

    if (r != 0)
    {
        threads.push_back(std::thread(S_Search, string, substring, std::ref(Iterators), left, string.size()));
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
}


int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> d(0, 3);
    
    std::vector <char> DNA = { 'A', 'G', 'T', 'C' };
    std::string st = "";

    int n;
    for (int i = 0; i < 1000; ++i) {
        n = d(gen);
        st = st + DNA[n];
    }
    std::cout << st << std::endl;
    
    
    std::string s_find;
    std::cout << "Find: ";
    std::cin >> s_find;
    
    
    std::vector<size_t> Iterators;
    parallel_search(st, s_find, Iterators);

    for (int i = 0; i < Iterators.size(); ++i) {
        std::cout << "On line: " << Iterators[i] << std::endl;
    }

    if (Iterators.size() == 0) {
        std::cout << "Not found" << std::endl;
    }

    return 0;
}


