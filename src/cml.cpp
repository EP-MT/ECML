#include "main.hpp"

namespace fs = std::filesystem;
using def = std::function<void()>;

void Help() {
    std::cout << "\nAvailable Commands:\n"
              << "exit - Exit the program\n"
              << "cls  - Clear the screen\n"
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
              << "lgn  - Logs in a user\n";
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

int main()
{
    std::cout << "Welcome to Ethans CommandLine(cml)!\n";
    std::string cur_dir = fs::current_path().string();

    ReadWrite rw;
    req rq;
    UAuth ua;
    MakeDelete mdr;

    std::unordered_map<std::string, def> commands = {
        {"lf", [&]() {mdr.ListDir(cur_dir); }},
        {"cd", [&]() {cur_dir = mdr.ChangeDir(cur_dir); }},
        {"mk", [&]() {mdr.Make();}},
        {"rm", [&]() {mdr.Delete();}},
        {"rnm", [&]() {mdr.Rename(cur_dir); }},
        {"help", Help},
        {"rd", [&]() {rw.Read();}},
        {"wr", [&]() {rw.Write();}},
        {"run", START},
        {"+usr", [&]() {ua.ADDUSR();}},
        {"-usr", [&]() {ua.DELUSR();}},
        {"lgn", [&]() {ua.LOGN();}},
        {"cls", [&]() { std::cout << std::string(50, '\n'); }},
        {"req", [&]() {rq.Request();}},
        {"check", [&]() {rq.parseWeb();}}
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