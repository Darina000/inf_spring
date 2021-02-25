#include <iostream>
#include <functional>
#include <boost/functional/hash.hpp>
#include <string>

#include <chrono>
#include <random>
#include <set>
#include <stdlib.h>

struct Struct{
    int a;
};


std::set < std::string > make_random_words(std::size_t N, std::size_t length) // length = 10 - good enough
{
    std::uniform_int_distribution <> letter(97, 122);
    std::default_random_engine e(static_cast < std::size_t > (
        std::chrono::system_clock::now().time_since_epoch().count()));

    std::set < std::string > words;

    for (std::string s(length, '_'); std::size(words) < N; words.insert(s))
        for (auto & c : s)
            c = letter(e);

    return words;
}




int main(int argc, char ** argv)
{
    std::set <std::size_t> Key;
    
    Struct one;
    
    int n = 2000;
    
        int Colis = 0;
    
        for (int i = 0; i<n; ++i){
            one.a = rand()%n;
            std::size_t N = 10;
            std::size_t length =  10;
            std::set < std::string > words =  make_random_words(N, length);
        
            if ((!Key.insert(boost::hash_value(one.a)).second)){
                ++Colis;
            }
            for (auto j: words){
                if(!Key.insert(boost::hash_value(j)).second){
                    ++Colis;
                }
            }
            
        }
    
    std::cout << n << " " << Colis << std::endl;
    return 0;
}

