#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

class ReadWrite
{
    private:
        std::string path = "";
        std::string contents = "";

    public:
        void Read()
        {
            std::cin >> path;

            if(fs::exists(path)){
                std::ifstream rfile(path);
        
                while(getline(rfile, contents)){
                    std::cout << contents << std::endl;
                }
        
                rfile.close();
            }
        
            else{
                std::cerr << "This file does not exist" << path << std::endl;
            }
        }

        void Write()
        {
            std::cin >> path;
            FuncGetChar(path);
        }

        void FuncGetChar(std::string file){
            std::fstream wfile(file, std::ios::app);
            std::string buffer;

            while (true) {
                int key = _getch();
                
                if (key == 27) {
                    break; 
                }
                
                if(key == 13){
                    wfile << buffer << std::endl;
                    std::cout << std::endl;
                    buffer.clear();
                }

                if(key == 8){
                    if(!buffer.empty()){
                        buffer.pop_back();
                        std::cout << "\b \b";
                    }
                }
                
                buffer += static_cast<char>(key);
                std::cout << static_cast<char>(key);
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            wfile.close();
        }
};