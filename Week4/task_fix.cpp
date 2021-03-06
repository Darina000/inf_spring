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
    for (auto k = 2; k < n; ++k) {
        if (n % k == 0){
            return 0;}
        else{
            return 1;}
    }
    return 0;
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
    auto x = std::find_if(std::begin(v1), std::end(v1), [](auto n) {return isPrime(n); });
    if (x == std::end(v1)){
            std::cout << "No prime numbers " << std::endl;
    }else{
        std::cout << *x << " numbers" << std::endl;}
    
    
    
    //9
    std::mt19937 rng; // default constructed, seeded with fixed seed
    std::vector <int> v2(v1.size());
    std::generate_n(v2.begin(), v1.size(), std::ref(rng));
    print(v2);
    
    
    //12
    std::vector <int> v3 = v2;
    std::transform (v3.begin(), v3.end(), v1.begin(), v3.begin(), std::minus<int>());
    print(v3);
    
    
    //16
    std::nth_element(v3.begin(), std::next(v3.begin()), v3.end(), std::greater<int>());
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
    size_t n = 1000000000;
    std::vector <int> v0(n);
    std::generate_n(v0.begin(), n, std::ref(rng));
    

    unsigned int num1 = 0;
   
    std::sort(std::begin(v0), std::end(v0),[&num1]( unsigned int lhs, unsigned int rhs )
              {++num1; return lhs < rhs;});
    
    std::cout << "in std::sort: " << num1 << std::endl;
   
    
    unsigned int num2 = 0;
    std::nth_element(v0.begin(), v0.begin()+1, v0.end(), [&num2]( unsigned int lhs, unsigned int rhs )
                     {++num2; return lhs < rhs;});
    
    std::cout << "in std::nth_element: " << num1 << std::endl;
    
    
    
    
     
    return 0;
}
