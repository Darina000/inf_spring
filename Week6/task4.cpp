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
   // nlohmann::json j;
    in >> point.name;
    in >> point.hight;
    in >> point.year;
 
    return in;
}

void inp(Person &p, nlohmann::json &j){
    std::cin >> p;
    j["name"] = p.name;
    j["hight"] = p.hight;
    j["year"] = p.year;
}


int main()
{
    
    std::filesystem::create_directories("/Users/darazemlanskaa/Documents/20-02sem 2/20-02sem/task4");
    std::filesystem::path path_output = "/Users/darazemlanskaa/Documents/20-02sem 2/20-02sem/task4/output.json";
    std::fstream fout(path_output.string(), std::ios::out);
    
    Person one, two;
    nlohmann::json j, q;
    inp(one, j);
    inp(two, q);
    
    nlohmann::json array = nlohmann::json::array({ q, j });
    
    for (auto &k : array){
        std::cout << k << "\n";
        fout << k << "\n";
    }
    
    
    return 0;
}
