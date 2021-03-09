#include <iostream>
#include <functional>
#include <boost/functional/hash.hpp>
#include <string>
#include <unordered_set>
#include <chrono>
#include <random>
#include <set>
#include <stdlib.h>

#include <boost/container_hash/hash.hpp>
#include  <boost/type_index/type_index_facade.hpp>


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

template < typename T >
void hash_combine(std::size_t & seed, const T & value) noexcept
{
   seed ^= std::hash < T > ()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template < typename T >
void hash_value(std::size_t & seed, const T & value) noexcept
{
   hash_combine(seed, value);
}

template < typename T, typename ... Types >
void hash_value(std::size_t & seed, const T & value, const Types & ... args) noexcept
{
   hash_combine(seed, value);
   hash_value(seed, args...);
}

template < typename ... Types >
std::size_t hash_value(const Types & ... args) noexcept
{
   std::size_t seed = 0;
   hash_value(seed, args...);
   return seed;
}

int main(int argc, char ** argv)
{
   std::set <std::size_t> Key;
   int n = 1000000;
   const std::size_t length =  10;
   const std::size_t N = 10;
   std::set < std::string > words =  make_random_words(N, length);
   int Colis = 0;
   
   for (int i = 0; i< n; ++i){
   for (auto j: words){
       int a = rand();
       if(!Key.insert(hash_value(j, a)).second){
           ++Colis;
       }
   }}
   
   std::cout << Colis << std::endl;
   return 0;
}
