#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include <random>
#include <cctype>
#include <cassert>
#include <string>
#include <utility>


void print(std::vector <int> & v){
    std::copy ( v.begin(), v.end(), std::ostream_iterator<int>{std::cout, " "} );
}


auto isPrime(int n) {
    if (n < 1){
        return 0;}
    for (auto k = 2; k < std::sqrt(n); ++k) {
        if (n % k == 0){
            return 0;}
        else{
            return 1;}
    }
    return 0;
}


bool f(std::vector <int>& v1){
    auto x = std::find_if(std::begin(v1), std::end(v1), [](auto n) {return isPrime(n); });
    return !(x == std::end(v1));
}


int main()
{
    
    std::vector <int> v1(10);

    //1
    std::iota(v1.begin(), v1.end(), 1);
    print(v1);
    
    
    
    //2
   
   std::copy(
              std::istream_iterator < int >(std::cin),
              std::istream_iterator < int >(),
              std::back_inserter(v1)
              );
    
    print(v1);
    
    
    
    //7
    std::cout << f(v1);
    
    
    
    //9
    std::vector<int> v2(v1.size());
    std::mt19937_64 m_gen;
    std::uniform_int_distribution<int> dist(1, 100);

    std::generate(v2.begin(), v2.end(), [&](){
        return dist(m_gen);
        });
    
    
    //12
    std::vector <int> v3 = v2;
    std::transform (v3.begin(), v3.end(), v1.begin(), v3.begin(), std::minus<int>());
    print(v3);
    
    
    //16
    std::nth_element(v3.begin(), std::next(v3.begin()), std::next(std::next(v3.begin())), std::greater<int>());
    std::cout << "top 3: " << v3[0] << " " << v3[1] << " " << v3[2] << std::endl;
    
    
    //18
    std::vector<int> v4;
    std::merge(
            v1.cbegin(), v1.cend(),
            v2.cbegin(), v2.cend(),
            std::back_inserter(v4));
    print(v4);
    
    
    //20
    std::copy ( v1.begin(), v1.end(), std::ostream_iterator<int>{std::cout, " "} );
    std::copy ( v2.begin(), v2.end(), std::ostream_iterator<int>{std::cout, " "} );
    std::copy ( v3.begin(), v3.end(), std::ostream_iterator<int>{std::cout, " "} );
    std::copy ( v4.begin(), v4.end(), std::ostream_iterator<int>{std::cout, " "} );
    
    
    //extra
    
    std::mt19937 rng; // default constructed, seeded with fixed seed
    size_t n = 2000000;
    std::vector <int> v0(n);
    std::generate_n(v0.begin(), n, std::ref(rng));
    std::vector <int> v00(n);
    v00 = v0;
    
    unsigned int num1 = 0;
   
    std::sort(std::begin(v0), std::end(v0),[&num1]( unsigned int lhs, unsigned int rhs )
              {++num1; return lhs < rhs;});
    
    std::cout << "in std::sort: " << num1 << std::endl;
    


    unsigned int num2 = 0;
    std::nth_element(v00.begin(), std::next(v00.begin()), v00.end(), [&num2]( unsigned int lhs, unsigned int rhs )
                     {++num2; return lhs < rhs;});
    
    std::cout << "in std::nth_element: " << num2 << std::endl;
    
    
    
    
    
     
    return 0;
}
