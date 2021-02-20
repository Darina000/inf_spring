#include <iostream>
#include <vector>
#include <deque>
#include <array>
#include <list>
#include <forward_list>
#include <algorithm>

int main()
{
    
    std::vector<int> v1 = { 1, 2, 3, 4, 5 };
    std::cout << v1.capacity() << std::endl; //5
    v1.push_back(3);
    std::cout << v1.capacity() << std::endl; // 10
    //емкость вектора увеличилась в два раза
    
    std::cout << " #2 " << std::endl;
    
    std::vector<int> v2;
    v2.reserve(5);
    std::cout << v2.capacity() << std::endl; // 5
    for (int i = 1; i< 7; ++i){
        v2.push_back(1);
    }
    std::cout << v2.capacity() << std::endl; // 10
    //емкость опять увеличилась в 2 раза
    
    std::cout << " #3 " << std::endl;
    std::vector<int> v3;
    try{
        v3.reserve(v3.max_size());
    }
    catch(...){
        std::cout << "exception" << std::endl;
        std::cout << v3.capacity() << std::endl;
    }
    //емкость 0, получили исключение
    

    return  0;
}
