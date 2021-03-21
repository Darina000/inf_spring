
#include <string>
#include <iostream>
#include <bitset>
#include <iomanip>
#include <sstream>
#include <iterator>
#include <string>
#include <fstream>



int main(){
    
  std::fstream fit("/Users/darazemlanskaa/Documents/20-02sem 2/20-02sem/test_new.txt", std::ios::out);
    std::fstream fi("/Users/darazemlanskaa/Documents/20-02sem 2/20-02sem/test.txt", std::ios::in);
    std::string str;
    bool flag = true;
    while(!fi.eof()){
        getline(fi, str);
        for (int i = 0; i< str.length(); ++i){
            if (!flag){
            }
            if ((str[i] == '/') && (str[i+1] == '/')){
                fit << '\n';
                break;
            }
            if ((str[i] == '/') && (str[i+1] == '*')){
                flag = false;
            }
            if ((str[i] == '*') && (str[i+1] == '/')){
                i+=2;
                flag = true;
            }
            
            if(flag){
                fit << str[i];
            }
        }
    }
    fi.close();
    
    return 0;
}
