#include <iostream>
#include <string>
#include <vector>
#include <map>

int main(){
    std::string DelimArray[5] = {"http://", "https://", "www.", "/", "."};
    std::vector<std::string> subs;

    std::string input, dot, sl;
    std::cout << "Delim Finder: ";
    std::cin >> input;

    for(int i = 0; i < 5; i++){
        int position = input.find(DelimArray[i]);
        int length = DelimArray[i].length();

        if(position != std::string::npos){
            subs.push_back(DelimArray[i]);
            input.erase(position, length);
            std::cout << position << " " << subs.back() << std::endl;
        }

        else{
            std::cout << "not found" << std::endl;
        }
    }

    std::cout << input << std::endl;
}