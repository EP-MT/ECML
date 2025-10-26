#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <unistd.h>
#include <csignal>
#include <map>
#include <functional>
#include <conio.h>
#include <thread> 
#include "json.hpp"
#include "rw.hpp"
#include "pac.hpp"

namespace fs = std::filesystem;
using def = std::function<void()>;
using json = nlohmann::json;
bool login_successful = false;

void ListDir(const std::string& cur_dir)
{
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

void Help() {
    std::cout << "\nAvailable Commands:\n"
              << "exit - Exit the program\n"
              << "cls - Clear the screen\n"
              << "lf   - List all files and directories\n"
              << "cd   - Change directories\n"
              << "mk   - Makes a file or directory\n"
              << "rm   - Deletes a file or directory\n"
              << "rnm  - Renames a file or Direcroy\n"
              << "rd   - Reads File\n"
              << "wr   - Writes to a file\n"
              << "run  - Starts a process or a progrm\n"
              << "help - Show this help message\n"
              << "+usr - Adds a user to the program\n"
              << "-usr - Deletes a user from the program\n"
              << "lgn - Logs in a user\n";
}

void START(){
    std::string file;
    std::cin >> file;

    const char* filec = file.c_str();

    if(fs::exists(filec)){
        if (access(filec, X_OK) == 0){
            try{
                const char* argv[] = {file.c_str(), nullptr};
                execv(file.c_str(), const_cast<char* const*>(argv));
            }

            catch(const fs::filesystem_error& e){
                std::cerr << "Error: " << e.what() << std::endl;
            };            
        }

        else{
            std::cout << "This File is not an executable:" << file << std::endl;
        }
    }

    else{
        std::cout << "This is file does not exist:" << file << std::endl;
    }

}

void ADDUSR(){
    std::string user, pass;

    std::cout << "Write Your Username here:";
    std::cin >> user;
    std::cout << "Write your password here:";
    std::cin >> pass;

    std::ifstream usrs("usrs.json");

    json credentials;
    usrs >> credentials;
    usrs.close();

    if(!credentials.contains("users")){
        credentials["users"] = json::array();
    }

    credentials["users"].push_back({{"username", user}, {"password", pass}});

    std::ofstream output_file("usrs.json");

    output_file << credentials.dump(4);
    output_file.close();
}

void LOGN(){
    std::string user, pass;
    std::cout << "Write Your Username here:";
    std::cin >> user;
    std::cout << "Write your password here:";
    std::cin >> pass;

    if(fs::exists("usrs.json")){
        std::ifstream usrs("usrs.json");
        json credentials;
        usrs >> credentials;
        usrs.close();

        if(credentials.contains("users")){
            for(const auto& u : credentials["users"]){
                if(u["username"] == user && u["password"] == pass)
                    std::cout << "Root login sucessful." << std::endl;
                    login_successful = true;
            }
        }

        else
        {
            std::cout << "Wrong usrename or password" << std::endl;
        }
    }

    else{
        std::cout << "You cant login because no credentials have been made yet" << std::endl << "Please you the +usr command to make credentials" << std::endl;
    }

}

void DELUSR(){
    std::string user, pass;
    std::cout << "Write Your Username here:";
    std::cin >> user;
    std::cout << "Write your password here:";
    std::cin >> pass;
    std::ifstream usrs("usrs.json");

    json credentials;
     if(credentials.contains("users")){
            for(const auto& u : credentials["users"]){
                if(u["username"] == user && u["password"] == pass){

                    credentials["user"] = nullptr;
                    credentials["password"] = nullptr;
            
                    std::ofstream output("usrs.json");
                    output << credentials.dump(4);
                    output.close();
            
                    std::cout << "user successfully deleted" << std::endl;
                }
            
                else
                {
                    std::cout << "You Entered a wrong password or username." << std::endl;
                }
            }
        }
}

int main()
{
    std::cout << "Welcome to Ethans CommandLine(cml)!\n";
    std::string cur_dir = fs::current_path().string();

    ReadWrite rw;
    req rq;

    std::map<std::string, def> commands = {
        {"lf", [&]() { ListDir(cur_dir); }},
        {"cd", [&]() { cur_dir = ChangeDir(cur_dir); }},
        {"mk", Make},
        {"rm", Delete},
        {"rnm", [&]() { Rename(cur_dir); }},
        {"help", Help},
        {"rd", [&]() {rw.Read();}},
        {"wr", [&]() {rw.Write();}},
        {"run", START},
        {"+usr", ADDUSR},
        {"-usr", DELUSR},
        {"lgn", LOGN},
        {"cls", [&]() { std::cout << std::string(50, '\n'); }},
        {"req", [&]() {rq.Request();}}
    };

    bool running = true;
    std::string choice;

    while(running){
        std::cout << cur_dir << ">";
        std::cin >> choice;
        
        if (choice == "exit") {
            std::cout << "Closing program.\n";
            running = false;
        } 
        
        else if (commands.find(choice) != commands.end()) {
            commands[choice]();
        } 
        
        else {
            std::cout << "Invalid choice! Please choose a valid option.\n";
        }
    };

    return 0;
}