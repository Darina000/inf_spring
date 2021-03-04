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
    for (int n : v){
        std::cout << n << ' ';
    }
    std::cout << std::endl;
}

bool is_simple(int n){
    return (int)std::pow(3.0, (n-1)) % n == 1;
}

void sq(std::vector <int> & v){
    for (int i = 0; i< v.size(); ++i){
        v[i] *= v[i];
    }
}


unsigned int rand(std::vector<int>& v)
{
    unsigned int seed = rand();
    seed = (rand() * seed + rand());
    return seed % size(v);
}
 


int main()
{
    std::vector <int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int a;
    std::cin >> a;
    
    std::fill_n(std::back_inserter(v1), 2, a);
    print(v1);
    
    std::random_device rd;
    std::mt19937 g(rd());
     
    std::shuffle(v1.begin(), v1.end(), g);
    print(v1);
    
    
    std::sort(v1.begin(), v1.end());
    v1.erase(std::unique(v1.begin(), v1.end()), v1.end());
    print(v1);
    
    auto result1 = std::count_if(std::begin(v1), std::end(v1), [](auto x) {return !(x % 2);});
    std::cout <<  result1 << std::endl;
    
    
    auto [min, max] = std::minmax_element(std::begin(v1), std::end(v1));
    std::cout << *min << ' ' << *max << std::endl;
    
    
    
    auto result = std::find_if(std::begin(v1), std::end(v1), [](int i){return is_simple(i);});
    if (result != std::end(v1)) {
            std::cout  << *result << '\n';
        }

    
    std::transform(std::begin(v1), std::end(v1), std::begin(v1), [](auto x){return x*x;});
    print(v1);
    std::cout << "v1" << '\n';
    
    std::vector <int> v2;
    std::mt19937 mersenne(rd()); // инициализируем Вихрь Мерсенна случайным стартовым числом
    for (int count = 0; count < size(v1); ++count)
        {
            v2.push_back(rand(v1));
        }
    print(v2);
    

    int sum = std::accumulate(v2.begin(), v2.end(), 0);
    std::cout << "sum: " << sum << '\n';

    std::fill_n(v2.begin(), 2, 1);
    std::cout << "v2" << '\n';
    print(v2);
    
    
    std::vector<int> v3;
    for(int i =0; i< v1.size(); ++i){
       // std::fill_n(v3.begin(), 1, v1[i]-v2[i]);
        v3.push_back(v1[i]-v2[i]);
    }
    
    //std::fill_n(v3.begin(), 2, 1);
    //std::adjacent_difference(v1.begin(), v1.end(), v2.begin());
    std::cout << "v3" << '\n';
    print(v3);
    std::cout << "v2" << '\n';
    print(v2);
    std::cout << "v1" << '\n';
    print(v1);
    
    
    std::replace_if(v3.begin(), v3.end(), [](int i){return i<0;}, 0);
    print(v3);
    
    
    v3.erase(std::remove_if(v3.begin(), v3.end(), [](int x){return x == 0;}),v3.end());
    print(v3);
    
    
    std::reverse(v3.begin(), v3.end());
    print(v3);
    
    
    
    std::nth_element(v3.begin(), v3.begin()+1, v3.end(), std::greater<int>());
    std::cout << "top 3: " << v3[0] << " " << v3[1] << " " << v3[2] << std::endl;
    
    
    std::sort(v1.begin(), v1.end());
    print(v1);
    std::sort(v2.begin(), v2.end());
    print(v2);
    
    
    std::vector<int> v4;
    std::set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), std::back_inserter(v4));
    print(v4);
    
    int value = 1;
    const auto p = std::equal_range(v4.begin(), v4.end(), value);
    for ( auto i = p.first; i != p.second; ++i ){
        std::cout << *i << ' ';
    }
    std::cout << std::endl;
    
    
    
    std::cout << "Final:" << std::endl;
    print(v1);
    print(v2);
    print(v3);
    
    
    
    //extra
    std::vector <int> v1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    unsigned int num1 = 0;
   
    std::sort(std::begin(v1), std::end(v1),[&num1]( unsigned int lhs, unsigned int rhs )
              {++num1; return lhs < rhs;});
    
    std::cout << "in std::sort: " << num1 << std::endl;
   
    
    unsigned int num2 = 0;
    std::nth_element(v1.begin(), v1.begin()+1, v1.end(), [&num2]( unsigned int lhs, unsigned int rhs )
                     {++num2; return lhs < rhs;});
    
    std::cout << "in std::nth_element: " << num1 << std::endl;
    return 0;
}
