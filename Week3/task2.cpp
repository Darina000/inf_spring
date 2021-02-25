#include <iostream>
#include <bitset>
#include "boost/lexical_cast.hpp"

unsigned int BKDRHash_float(const float f)
{
std::string s = boost::lexical_cast<std::string>(f);
unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
unsigned int hash = 0;
unsigned int i = 0;

for (char &c : s)
{
    hash = (hash * seed) + c;
}

return hash;
}


int main()
{
    std::cout << BKDRHash_float(3.28) << std::endl;
    return 0;
}
