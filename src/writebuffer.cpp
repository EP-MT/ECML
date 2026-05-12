#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

int main(){
    std::string file;
    
    while(true){
        std::cin >> file;

        if(fs::exists(file)){
            buffer(file);
        }
        else{
            std::cout << "file doesnt exist try again" << std::endl;
        }
    }
}

void buffer(std::string UserFile){
    std::fstream file(UserFile, std::ios::in | std::ios::out);
    std::string buffer;

    file.clear();
    file.seekp(0, std::ios::end);
    char c;
    while(true){
        c = std::cin.get();

        if(c == '\b' || c == 127){
            std::streampos pos = file.tellp();
            if(pos > 0){
                file.seekp(pos - 1);
                file.put(' ');
                file.seekp(pos - 1);
                std::cout << "\b \b";
            }
        }
        else{
            file.put(c);
            std::cout << c;
        }
        file.flush();
    }
}