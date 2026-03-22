#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "usrAuth.hpp"

namespace fs = std::filesystem;

class MakeDelete{
    private:
        std::string cur_dir = fs::current_path().string();

    public:
        void Make(){
            std::string choice;
            std::string name;
        
            std::cout << "How would you like to create your file?\n"
                    << ".txt\n"
                    << "directory\n";    
        
            std::cin >> choice;
        
            std::cout << "Enter Name:";
            std::cin >> name;
        
            try{
                if(choice == "txt"){
                    std::ofstream newfile(name + ".txt");
                    newfile << "";
                    newfile.close();
        
                    if(newfile){
                        std::cout << "new text file created:" << name << std::endl;
                    }
        
                    else{
                        std::cout << "Error creating file:" << name << std::endl;
                    }
                }
        
                else if(choice == "folder"){
                    if(fs::create_directory(name)){
                        std::cout << "new directory created:" << name << std::endl;
                    }
        
                    else{
                        std::cout << "Error creating directory:" << name << std::endl;
                    }
                }
        
                else{
                    std::cout << "Invalid choice! Please choose a valid option.\n";  
                }
            }
        
            catch(const fs::filesystem_error& e){
                std::cerr << "Error: " << e.what() << std::endl;
            };
        
        
        }
        
        void Delete(){
            std::string name;
            bool exists = false;
        
            std::cin >> name;
            if(login_successful){
                if(!fs::exists(name)){
                    std::cout << "File or Directory does not exists:" << name << std::endl;
                }
        
                try{
                    if(fs::is_directory(name)){
                        fs::remove_all(name);
                        std::cout << "Directory successfully Removed:" << name << std::endl;
                    }
        
                    else{
                        fs::remove(name);
                        std::cout << "File Successfully removed:" << name << std::endl;
                    }
                }
        
                catch(const fs::filesystem_error& e){
                    std::cerr << "Error: " << e.what() << std::endl;
                };
            }
        
            else{
                std::cout << "You do not have Root access" << std::endl;
            }
        }

        void Rename(const std::string& cur_dir){
            std::string name;
            std::string new_name;
            bool exists = false;

            std::cin >> name;

            if(!fs::exists(name)){
                std::cout << "File or Directory does not exists:" << name << std::endl;
            }

            std::cout << "What is the new name for the file or directory:";
            std::cin >> new_name;

            std::string name_dir = fs::current_path().string() + "/" + name;
            std::string newname_dir = fs::current_path().string() + "/" + new_name;
            try{
                fs::rename(name_dir, newname_dir);
                std::cout << "The File or directory has been renamed to:" << new_name << std::endl;
            }

            catch(const fs::filesystem_error& e){
                std::cerr << "Error: " << e.what() << std::endl;
            };
        }

        void ListDir(const std::string& cur_dir){
            try{
                std::cout << "Listing files and directories in: " << cur_dir << "\n";
                for(const auto& entry : fs::directory_iterator(cur_dir)){
                    std::cout << (entry.is_directory() ? "[DIR] " : "[FILE]")
                            << entry.path().filename().string() << std::endl;
                }
            }

            catch(fs::filesystem_error& e){
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

        std::string ChangeDir(const std::string& cur_dir){
            std::string newpath;
            std::cin >> newpath;


            if(newpath.empty()){
                std::cout << "Invalid Path" << std::endl;
                return cur_dir;
            }


            try{
                fs::current_path(newpath);
                return fs::current_path().string();
            }

            catch(const std::exception& e){
                std::cerr << "Error: " << e.what() << std::endl;
                return cur_dir;
            };
        }
};