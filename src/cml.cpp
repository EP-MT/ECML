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
              << "help - Show this help message\n"
              << "+usr - Adds a user to the program\n"
              << "-usr - Deletes a user from the program\n"
              << "lgn  - Logs in a user\n";
}

int main()
{
    std::cout << "Welcome to Ethans CommandLine(cml)!\n";
    std::string cur_dir = fs::current_path().string();

    ReadWrite rw;
    UAuth ua;
    MakeDelete mdr;
    win window;
    //req rq;

    std::unordered_map<std::string, def> commands = {
        {"lf", [&]() {mdr.ListDir(cur_dir); }},
        {"cd", [&]() {cur_dir = mdr.ChangeDir(cur_dir); }},
        {"mk", [&]() {mdr.Make();}},
        {"rm", [&]() {mdr.Delete();}},
        {"rnm", [&]() {mdr.Rename(cur_dir); }},
        {"help", Help},
        {"rd", [&]() {rw.Read();}},
        {"wr", [&]() {rw.Write();}},
        {"+usr", [&]() {ua.ADDUSR();}},
        {"-usr", [&]() {ua.DELUSR();}},
        {"lgn", [&]() {ua.LOGN();}},
        {"cls", [&]() { std::cout << std::string(50, '\n'); }}
        //,{"req", [&]() {rq.Request();}},
        //{"check", [&]() {rq.parseWeb();}}
    };

    bool running = true;
    std::string choice;

    window.pullWindow(cur_dir);
    
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