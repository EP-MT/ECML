#include "main.hpp"

namespace fs = std::filesystem;
using def = std::function<void()>;

void Help() {
    std::cout << "\nAvailable Commands:\n"
              << "exit - Exit the program\n"
              << "cls  - Clear the screen\n"
              << "lf   - List all files and directories\n"
              << "cd   - Change directories (Path)\n"
              << "mk   - Makes a file (Name)\n"
              << "mkd  - Makes a directory (Name)\n"
              << "rm   - Deletes a file or Directory (Name)\n"
              << "rnm  - Renames a file or Directory (Name, New Name)\n"
              << "rd   - Reads File (File Name)\n"
              << "wr   - Writes to a file\n (File Name)"
              << "help - Show this help message\n"
              << "+usr - Adds a user to the program\n"
              << "-usr - Deletes a user from the program\n"
              << "lgn  - Logs in a user\n";
}

int main()
{
    std::cout << "Welcome to CommandLine(cml)\n";
    std::string cur_dir = fs::current_path().string();

    ReadWrite rw;
    UAuth ua;
    MakeDelete mdr;
    win window;
    bool running = true;
    std::vector<std::string> args;

    auto validateArgs = [&](std::size_t minimum,
                            std::size_t maximum,
                            const std::string& usage) {
        if (args.size() < minimum || (maximum != 0 && args.size() > maximum)) {
            std::cerr << "Usage: " << usage << '\n';
            return false;
        }
        return true;
    };

    std::unordered_map<std::string, def> commands = {
        {"lf", [&]() {if (!validateArgs(0, 0, "lf")) return; mdr.ListDir(cur_dir);}},
        {"cd", [&]() {if (!validateArgs(1, 1, "cd <path>")) return; cur_dir = mdr.ChangeDir(cur_dir, args[0]);}},
        {"mk", [&]() {if (!validateArgs(2, 2, "mk <txt|folder> <name>")) return; mdr.Make(args[0]);}},
        {"mkd", [&](){if (!validateArgs(2, 1, "mkd <name>")) return; mdr.Makedir(args[0]);}},
        {"rm", [&]() {if (!validateArgs(1, 1, "rm <name>")) return; mdr.Delete(args[0]);}},
        {"rnm", [&]() {if (!validateArgs(2, 2, "rnm <old-name> <new-name>")) return; mdr.Rename(cur_dir, args[0], args[1]);}},
        {"help", [&]() {if (!validateArgs(0, 0, "help")) return; Help();}},
        {"rd", [&]() {if (!validateArgs(1, 1, "rd <path>")) return; rw.Read(args[0]);}},
        {"+usr", [&]() {if (!validateArgs(2, 2, "+usr <user> <pass>")) return; ua.ADDUSR(args[0], args[1]);}},
        {"-usr", [&]() {if (!validateArgs(2, 2, "-usr <user> <pass>")) return; ua.DELUSR(args[0], args[1]);}},
        {"lgn", [&]() {if (!validateArgs(2, 2, "lgn <user> <pass>")) return; ua.LOGN(args[0], args[1]);}},
        {"cls", [&]() {if (!validateArgs(0, 0, "cls")) return; std::cout << std::string(50, '\n');}},
        {"exit", [&]() {if (!validateArgs(0, 0, "exit")) return; std::cout << "Closing program.\n";running = false;}},
        {"wr", [&]() {if (!validateArgs(2, 0, "wr <path> <text...>")) return;
            std::string text = args[1];
            for (std::size_t i = 2; i < args.size(); ++i) {
                text += " " + args[i];
            }
            rw.Write(args[0], text);
        }}
    };

    auto dispatch = [&](const std::string& line) {
        std::istringstream input(line);
        std::string choice;
        if (!(input >> std::quoted(choice))) {
            return running;
        }

        args.clear();
        std::string arg;
        while (input >> std::quoted(arg)) {
            args.push_back(arg);
        }

        const auto command = commands.find(choice);
        if (command != commands.end()) {
            command->second();
        }
        else {
            std::cerr << "Invalid choice! Please choose a valid option.\n";
        }
        return running;
    };

    window.pullWindow(cur_dir, dispatch);
    return 0;
}