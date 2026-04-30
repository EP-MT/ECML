#include <iostream>
#include <fstream>
#include <conio.h>
#include <filesystem>

namespace fs = std::filesystem;

int main(){
    std::string file = "test.txt";
    std::fstream wfile(file, std::ios::app);
    std::string buffer;
    std::string CurDir = fs::current_path().string();
    CurDir = "C:\\Users\\porte\\ALLCODE";
      
    while(true){
        int key = _getch();

        std::cout << key;

        if(key == 27){
            break;
        }
        
        if(key == 65){
                wfile << "im pressing a" << std::endl;
        }
    }
}