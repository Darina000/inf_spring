#include <iostream>
#include <sstream>
#include <regex>
#include <iomanip>
#include <string>
 

int main()
{
    std::regex time(R"(([0-2]?\d):[0-5]?\d:[0-5]?\d)");
    std::regex dat(R"((([1-2]\d{3})-(([1][0-2])|([0]?[1-9]))-[0-31]?\d))");
    
    const std::string data = "wede2001-04-01dgdr12:00:23hb";
    
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
