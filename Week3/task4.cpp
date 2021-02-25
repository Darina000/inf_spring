#include <iostream>
#include <functional>
#include <boost/functional/hash.hpp>
#include <string>
#include <vector>
#include <chrono>
#include <random>
#include <set>
#include <stdlib.h>



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



unsigned int RSHash(std::string str)
{
   unsigned int b    = 378551;
   unsigned int a    = 63689;
   unsigned int hash = 0;
   unsigned int i    = 0;
    unsigned int len = str.length();

   for (i = 0; i < len; ++i)
   {
      hash = hash * a + (str[i]);
      a    = a * b;
   }

   return hash;
}



unsigned int JSHash(std::string str)
{
   unsigned int hash = 1315423911;
   unsigned int i    = 0;
    unsigned int length = str.length();

   for (i = 0; i < length; ++i)
   {
      hash ^= ((hash << 5) + (str[i]) + (hash >> 2));
   }

   return hash;
}


unsigned int PJWHash(std::string str)
{
   const unsigned int BitsInUnsignedInt = (unsigned int)(sizeof(unsigned int) * 8);
   const unsigned int ThreeQuarters     = (unsigned int)((BitsInUnsignedInt  * 3) / 4);
   const unsigned int OneEighth         = (unsigned int)(BitsInUnsignedInt / 8);
   const unsigned int HighBits          =
                      (unsigned int)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
    
    unsigned int length = str.length();
   unsigned int hash = 0;
   unsigned int test = 0;
   unsigned int i    = 0;

   for (i = 0; i < length;  ++i)
   {
      hash = (hash << OneEighth) + (str[i]);

      if ((test = hash & HighBits) != 0)
      {
         hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
      }
   }

   return hash;
}

unsigned int ELFHash(std::string str)
{
   unsigned int hash = 0;
   unsigned int x    = 0;
   unsigned int i    = 0;
    unsigned int length = str.length();

   for (i = 0; i < length; ++i)
   {
      hash = (hash << 4) + (str[i]);

      if ((x = hash & 0xF0000000L) != 0)
      {
         hash ^= (x >> 24);
      }

      hash &= ~x;
   }

   return hash;
}


unsigned int BKDRHash(std::string str)
{
   unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
   unsigned int hash = 0;
   unsigned int i    = 0;
    unsigned int length = str.length();

   for (i = 0; i < length; ++i)
   {
      hash = (hash * seed) + (str[i]);
   }

   return hash;
}



unsigned int SDBMHash(std::string str)
{
   unsigned int hash = 0;
   unsigned int i    = 0;
    unsigned int length = str.length();

   for (i = 0; i < length; ++i)
   {
      hash = str[i] + (hash << 6) + (hash << 16) - hash;
   }

   return hash;
}


unsigned int DJBHash(std::string str)
{
   unsigned int hash = 5381;
   unsigned int i    = 0;
    unsigned int length = str.length();

   for (i = 0; i < length; ++i)
   {
      hash = ((hash << 5) + hash) + (str[i]);
   }

   return hash;
}


unsigned int DEKHash(std::string str)
{
    unsigned int length = str.length();
   unsigned int hash = length;
   unsigned int i    = 0;
    

   for (i = 0; i < length; ++i)
   {
      hash = ((hash << 5) ^ (hash >> 27)) ^ (str[i]);
   }

   return hash;
}


unsigned int APHash(std::string str)
{
    unsigned int length = str.length();
   unsigned int hash = 0xAAAAAAAA;
   unsigned int i    = 0;

   for (i = 0; i < length; ++i)
   {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (str[i]) * (hash >> 3)) :
                               (~((hash << 11) + ((str[i]) ^ (hash >> 5))));
   }

   return hash;
}


int main(int argc, char ** argv)
{
    std::set <unsigned int> Key;
    

        
        
        int n = 1000000;
        int Colis = 0;
        for (int i = 0; i<n; ++i){
            std::size_t N = 10;
            std::size_t length =  10;
            std::set < std::string > words =  make_random_words(N, length);
            
            for (auto i : words){
                if(!Key.insert(APHash(i)).second){
                    ++Colis;
                }
            }
        }
        std::cout << n << " " << Colis << std::endl;
    
    
    return 0;
}
