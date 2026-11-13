#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "winHandler.hpp"

namespace fs = std::filesystem;
class ReadWrite
{
    public:
        void Read(const std::string& path)
        {
            if (fs::exists(path)) {
                std::ifstream rfile(path);
                if (!rfile) {
                    std::cerr << "Unable to open file: " << path << std::endl;
                    return;
                }

                std::string contents;
                while (std::getline(rfile, contents)) {
                    std::cout << contents << std::endl;//
                }
            }
            else {
                std::cerr << "This file does not exist: " << path << std::endl;//
            }
        }

        void Write(const std::string& path, const std::string& text)
        {
            std::ofstream wfile(path, std::ios::app);
            if (!wfile) {
                std::cerr << "Unable to open file for writing: " << path << std::endl;
                return;
            }

            wfile << text << std::endl;
        }
};