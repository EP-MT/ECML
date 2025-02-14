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
#include <python3.12/Python.h>
#include <conio.h>

namespace fs = std::filesystem;
using def = std::function<void()>;

void LS(const std::string& cur_dir)
{
    try{
        std::cout << "Listing files and directories in: " << cur_dir << "\n";
        for(const auto& entry : fs::directory_iterator(cur_dir)){
            std::cout << (entry.is_directory() ? "[DIR] " : "[FILE] ")
                      << entry.path().filename().string() << std::endl;
        }
    }
    
    catch(fs::filesystem_error& e){
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

std::string CD(const std::string& cur_dir){

    std::string newpath;
    
    try{
        std::cin >> newpath;

        try
        {
            fs::current_path(newpath);
            return fs::current_path().string();

        }
        catch(const std::exception& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            return cur_dir;

        }
    }

    catch(const fs::filesystem_error& e){
        std::cerr << "Error: " << e.what() << std::endl;
        return cur_dir;
    };


}

void MK(){
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

void DEL(){
    std::string name;
    bool exists = false;

    std::cin >> name;

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

void RENAME(const std::string& cur_dir){
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

void HELP() {
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

void CAT(){
    std::string file;
    std::string contents;

    std::cin >> file;

    if(fs::exists(file)){
        std::ifstream rfile(file);

        while(getline(rfile, contents)){
            std::cout << contents << std::endl;
        }

        rfile.close();
    }

    else{
        std::cerr << "This file does not exist" << file << std::endl;
    }

}

int pid = 0;

int START(){
    std::string file;
    std::cin >> file;

    const char* filec = file.c_str();

    if(fs::exists(filec)){
        if (access(filec, X_OK) == 0){
            try{
                pid = getpid();
                const char* argv[] = {file.c_str(), nullptr};
                execv(file.c_str(), const_cast<char* const*>(argv));
                return pid;
            }

            catch(const fs::filesystem_error& e){
                std::cerr << "Error: " << e.what() << std::endl;
                return 0;
            };            
        }

        else{
            std::cout << "This File is not an executable:" << file << std::endl;
            return 0;
        }
    }

    else{
        std::cout << "This is file does not exist:" << file << std::endl;
        return 0;
    }

}

void KILL(){

}

void ECHO(){
    std::string file;
    std::string Wcontents;

    std::cin >> file;

    if(fs::exists(file)){
        std::fstream wfile(file, std::ios::app);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        while(true){
            int key = _getch();
            if(key == 27){
                break;
            }
            
            if(key == 13){
                wfile << std::endl;
                std::cout << std::endl;
            }
            
        wfile << static_cast<char>(key);
        std::cout << static_cast<char>(key);
    }
        wfile.close();
    }

    else{
        std::cerr << "This file does not exist" << file << std::endl;
    }
}

void ADDUSR(){
    std::string user;
    std::string pass;

    std::cout << "Write Your Username here:";
    std::cin >> user;
    std::cout << "Write your password here:";
    std::cin >> pass;

    std::fstream usrs("usrs.csv", std::ios::app);

    usrs << "\n" << user << "," << pass << std::endl;
    usrs.close();
}

void LOGN(){
    std::string user;
    std::string pass;

    std::string usrs;
    std::string spass;
    bool login_successful = false;

    std::cout << "Write Your Username here:";
    std::cin >> user;
    std::cout << "Write your password here:";
    std::cin >> pass;

    std::ifstream usrf("usrs.csv");

    while (getline(usrf, usrs, ',') && getline(usrf, spass)) {
        if (!spass.empty() && spass.back() == '\n') {
            spass.pop_back();
        }

        else if (usrs == user && spass == pass) {
            login_successful = true;
        }
    }

    if (login_successful) {
        std::cout << "Login successful!" << std::endl;
    } else {
        std::cout << "Invalid username or password." << std::endl;
    }
}

void DELUSR() {
    std::fstream usrs("usrs.csv", std::ios::in);
    std::string s;

    while(getline(usrs, s)){
        std::cout << s << std::endl;
    }

    std::string user;
    
    if(fs::exists("usrs.csv")){
        std::string temp = "temp.csv";
        std::ofstream tempf(temp);

        std::cout << "Write the username you want to delete:";
        std::cin >> user;

        usrs.seekg(0, std::ios::beg);

        while(getline(usrs, s)){
            if(s.find(user) == std::string::npos){
                tempf << s << std::endl;
            }
        }

        usrs.close();
        tempf.close();

        fs::remove("usrs.csv");
        fs::rename(temp, "usrs.csv");
    }
}

void PYSCRIPT(){
    std::string script;
    std::cin >> script;

    try{
        if(fs::exists(script)){
            Py_Initialize;
            std::ifstream script_file(script);
            std::string script_content((std::istreambuf_iterator<char>(script_file)),std::istreambuf_iterator<char>());
            Py_Finalize;
        }
    }

    catch(const fs::filesystem_error& e){
        std::cerr << "Error: " << e.what() << std::endl;
    };
}
int main()
{
    int Pidbuffer;
    std::cout << "Welcome to Ethans CommandLine(cml)!\n";
    std::string cur_dir = fs::current_path().string();

    std::map<std::string, def> commands = {
        {"lf", [&]() { LS(cur_dir); }},
        {"cd", [&]() { cur_dir = CD(cur_dir); }},
        {"mk", MK},
        {"rm", DEL},
        {"rnm", [&]() { RENAME(cur_dir); }},
        {"help", HELP},
        {"rd", CAT},
        {"wr", ECHO},
        {"run", START},
        {"+usr", ADDUSR},
        {"-usr", DELUSR},
        {"lgn", LOGN},
        {"cls", []() { std::cout << std::string(50, '\n'); }},
    };

    bool running = true;
    std::string choice;

    while(running){
        std::cout << cur_dir << ">";
        std::cin >> choice;

        if (choice == "exit") {
            std::cout << "Closing program.\n";
            running = false;
        } else if (commands.find(choice) != commands.end()) {
            commands[choice]();
        } else {
            std::cout << "Invalid choice! Please choose a valid option.\n";
        }
    }

    return 0;
}