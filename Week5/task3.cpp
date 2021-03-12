#include <iostream>
#include <regex>
#include <string>

int main() {

    std::string emailText = "Email address: rai@gmail.com, Trainer@grimm-jaud.de, wda-&hhj8*@mail.ru, 3-4j;h@ukg.um, 7gu+j@frac.tu, min@yandex.ru";
    
    
    std::string regExprStr(R"((([\w][0-9A-z.]+[\w]))@([_[:alpha:]]+\.)+[_[:alpha:]]{2,6})");
     std::regex rgx(regExprStr);

     std::smatch smatch;
   
    
    std::cout << "Email adress: " << emailText << std::endl;

    std::sregex_iterator begin(emailText.cbegin(), emailText.cend(), rgx);
    std::sregex_iterator end;

    std::for_each(begin, end, [](const std::smatch& m) {
                std::cout << m[3] << std::endl;
            });
    
    return 0;
}
