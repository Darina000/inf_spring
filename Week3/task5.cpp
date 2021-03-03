
#include <iostream>
#include <string>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>

using namespace boost::multi_index;

class Dictionary
{
public:
    struct Rec
    {
        std::string name;
        int number;
        
        
        
        friend std::ostream &operator<<(std::ostream &o, const Rec &note)
        {
            return o << "name: " << note.name << "number: " << note.number;
        }
    };
    
    void print()
    {
        for (auto record : data.get<IndexType::ORD>())
        {
            std::cout << record << std::endl;
        }
    }

    void insert(const Rec &note)
    {
        this->data.insert(note);
    }

    const Rec &operator[](std::size_t idx) const { return this->data.get<IndexType::RAND>()[idx]; }

    Rec find(std::string name)
    {
        return *this->data.get<IndexType::HASH>().find(name);
    }

private:
    enum IndexType
    {
        HASH,
        ORD,
        RAND
    };

    multi_index_container<
        Rec, indexed_by<
                 hashed_non_unique<
                     member<Rec, std::string, &Rec::name>>,
                 ordered_non_unique<
                     member<Rec, std::string, &Rec::name>>,
                 random_access<>>>
        data;
};

int main()
{
    Dictionary d;
    d.insert({ "Basilisa", 34 });
    d.insert({ "Minerva", 1 });
    d.insert({ "Fedor", 12 });

    
    std::cout << "Random" << d[0]   << std::endl;
    
    
    std::cout << "Sorted" << std::endl;
    d.print();

    
    std::cout << "Search" << d.find("Fedor") << std::endl;


    return 0;
}
