#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>


int main() {
    
    std::ofstream file("/Users/darazemlanskaa/Documents/20-02sem 2/20-02sem/set.txt");

    const int size = 10;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < size; j++) {
            file << "line number " << j << ": " << i << std::endl;;
        }
        file << '\n';
    }

    int n;
    std::cout << "Enter number of line: ";
    std::cin >> n;

    std::ifstream ffile("set.txt");

    if (n > 1)
        ffile.seekg((n - 1) * (size + 2), std::ios::beg);
    else
        ffile.seekg(0); // at the begining of the file

    std::string str;
    getline(ffile, str);
    std::cout << "line: " << str << std::endl;
    return 0;
}
