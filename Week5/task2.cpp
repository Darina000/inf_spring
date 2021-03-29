#include <iostream>
#include <boost/locale.hpp>
#include <bitset>
#include <string>
#include <locale>
#include <Windows.h>
#include <vector>
std::string convert_locale_to_utf(const std::string& string)
{
    boost::locale::generator generator;
    generator.locale_cache_enabled(true);

    std::locale locale =
        generator(boost::locale::util::get_system_locale());

    return boost::locale::conv::to_utf < char >(string, locale);
}

std::string convert_utf_to_locale(const std::string& string)
{
    boost::locale::generator generator;
    generator.locale_cache_enabled(true);

    std::locale locale =
        generator(boost::locale::util::get_system_locale());

    return boost::locale::conv::from_utf < char >(string, locale);
}

int hash(char32_t elem) {
    int number = static_cast<int>(elem);
    number -= 224;
    return number;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::vector<std::pair<char32_t, char32_t>> convertable = { std::make_pair('a','0'),std::make_pair('b','0'),std::make_pair('v','0'),std::make_pair('g','0'),
        std::make_pair('d','0'),std::make_pair('e','0'),std::make_pair('z','h'),std::make_pair('z','0'),std::make_pair('i','0'),std::make_pair('i','0'),
        std::make_pair('k','0'),std::make_pair('l','0'),std::make_pair('m','0'),std::make_pair('n','0'),std::make_pair('o','0'),std::make_pair('p','0'),
        std::make_pair('r','0'),std::make_pair('s','0'),std::make_pair('t','0'),std::make_pair('u','0'),std::make_pair('f','0'),std::make_pair('k','h'),
        std::make_pair('t','s'),std::make_pair('c','h'),std::make_pair('s','h'),std::make_pair('1','0'),std::make_pair('i','e'),std::make_pair('y','0'),std::make_pair('0','0')
        ,std::make_pair('e','0'),std::make_pair('i','u'),std::make_pair('i','a') };
    std::string line;
    std::cin >> line;
    std::string u8string = convert_locale_to_utf(line);
    std::u32string u32string =boost::locale::conv::utf_to_utf < char32_t, char > (u8string);
    std::u32string newline;
    for (auto c : u32string) {
        int a = hash(c);
        if (a >= 0 && a < 32) {
            if (convertable[a].first != '0' && convertable[a].first != '1') {
                if (convertable[a].second != '0') {
                    newline.push_back(convertable[a].first);
                    newline.push_back(convertable[a].second);
                }
                else {
                    newline.push_back(convertable[a].first);
                }
            }
            if (convertable[a].first == '1') {
                newline.push_back(convertable[a - 2].first);
                newline.push_back(convertable[a - 2].second);
                newline.push_back(convertable[a - 1].first);
                newline.push_back(convertable[a - 1].second);
            }
        }
        else {
            newline.push_back(c);
        }
    }
    std::string newline8 = boost::locale::conv::utf_to_utf < char, char32_t >(newline);
    std::string nline = convert_utf_to_locale(newline8);
    std::cout << nline;
    
}
