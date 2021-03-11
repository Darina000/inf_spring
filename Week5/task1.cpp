#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include <string>
void show_currency_symbol(const std::string& locale_name) {
    std::cout << locale_name << " : " <<
        std::use_facet < std::moneypunct < char, false > >(
            std::locale(locale_name)).curr_symbol() << std::endl;
}

int main() {

    const int p = 73.27;

    std::cin.imbue(std::locale("en_US.UTF-8"));
    std::string value;
    std::cin >>value;
    
    
    std::cout << std::fixed << std::setprecision(2);
    std::istringstream s1(value);
    s1.imbue(std::locale("en_US.UTF-8"));
    long double val;
    s1 >> std::get_money(val);
    std::cout << val/100 << std::endl;
    
    val *= p;
    std::cout << val/100 << std::endl;
    
    std::ostringstream ssout;
    ssout.imbue(std::locale("ru_RU.UTF-8"));
    ssout << std::showbase << std::put_money(val, false) << std::endl;
    std::cout << ssout.str() << std::endl;
    return 0;
}

