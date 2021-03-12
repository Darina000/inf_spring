#include <iostream>
#include <sstream>
#include <regex>
#include <iomanip>
#include <string>
 

int main()
{
    std::regex time(R"((\d{2})(:[0-5]\d){2})");
    std::regex dat(R"((([1-2]\d{3})-(([1][0-2])|([0][1-9]))-(([0-2]\d)|([3][0,1]))))");
    
    const std::string data = "wede2001-04-30dgdr12:20:23hb";
    
    std::sregex_iterator begin(data.cbegin(), data.cend(), dat);
    std::sregex_iterator end;
    
    std::sregex_iterator begin_t(data.cbegin(), data.cend(), time);
    std::sregex_iterator end_t;
    

    std::for_each(begin, end, [](const std::smatch & m)
    {
        std::cout << m[0] << std::endl;
    });
    std::for_each(begin_t, end_t, [](const std::smatch & m)
    {
        std::cout << m[0] << std::endl;
    });
    return 0;
    
}
