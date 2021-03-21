#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"


class Person
{
public:
    std::string name;
    double hight;
    int year;
 
public:
    Person(std::string name = "Noname", double hight=0.0, int year=20): name(name), hight(hight), year(year)
    {
    }
 
    friend std::ostream& operator<< (std::ostream &out, const Person &point);
    friend std::istream& operator>> (std::istream &in, Person &point);
};
 
std::ostream& operator<< (std::ostream &out, const Person &point)
{
    out << "Person(" << point.name << ", " << point.hight << ", " << point.year << ")";
    return out;
}
 
std::istream& operator>> (std::istream &in, Person &point)
{
    nlohmann::json j;
    in >> point.name;
    in >> point.hight;
    in >> point.year;
 
    return in;
}


int main()
{
    std::filesystem::create_directories("/Users/darazemlanskaa/Documents/20-02sem 2/20-02sem/task4");
    std::filesystem::path path_output = "/Users/darazemlanskaa/Documents/20-02sem 2/20-02sem/task4/output.txt";
    std::fstream fout(path_output.string(), std::ios::out);
    
    Person one;
    std::cin >> one;
    
    nlohmann::json j;
    j["name"] = one.name;
    j["hight"] = one.hight;
    j["year"] = one.year;
    
    for (auto &[key, value] : j.items()) // C++17 bindings
    {
        std::cout << key << " : " << value << "\n";
    }
    fout << std::setw(4) << j;
    
    
    Person two;
    std::cin >> two;
    
    nlohmann::json q;
    q["name"] = two.name;
    q["hight"] = two.hight;
    q["year"] = two.year;
    
    for (auto &[key, value] : q.items()) // C++17 bindings
    {
        std::cout << key << " : " << value << "\n";
    }
    fout << std::setw(4) << q;
    
    
    
    
    return 0;
}
