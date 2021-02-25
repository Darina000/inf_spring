#include <iostream>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

using namespace boost::multi_index;

struct Rec
{
    std::string name;
    int number;
    struct ByName {}; struct ByPhone {}; struct ByAddr {};
};

using Rec_multi_index =  multi_index_container<Rec,
    indexed_by<
        
        ordered_unique<
            member <Rec, std::string, &Rec::name>>,
        ordered_non_unique<
            member<Rec, int, &Rec::number>>,
        random_access<>,
        hashed_non_unique <
            member < Rec, int, &Rec::number > >
    >
>;

int main(){
    Rec_multi_index people;
    Rec r1 = { "Basilisa", 34 };
    Rec r2 = { "Minerva", 1 };
    Rec r3 = { "Fedor", 12 };
    
    std::cout << "ok1 " << people.insert(r1).second << std::endl; // ok1 true
    std::cout << "ok2 " << people.insert(r1).second << std::endl; // ok2 false
    people.insert(r2);
    people.insert(r3);

    std::string find_id = "Basilisa";
    std::cout << people.count(find_id) << std::endl;
    
    
    auto & hashed_index = people.get < 1 > ();
    std::cout << hashed_index.count(4) << std::endl;

    const auto & random_access_index = people.get < 2 > ();
    std::cout << random_access_index[0].name << std::endl;

    return 0;
}

