#ifndef USR_AUTH_HPP
#define USR_AUTH_HPP
#include <iostream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

using json = nlohmann::json;
namespace fs  = std::filesystem;

extern bool login_successful;

class UAuth{
    public:        
        void ADDUSR(){
            std::string user, pass;

            std::cout << "Write Your Username here:";
            std::cin >> user;
            std::cout << "Write your password here:";
            std::cin >> pass;
            json credentials;

            std::ifstream usrs("usrs.json");
        
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

            if(fs::exists("C:\\Users\\porte\\ALLCODE\\EPOS\\usrs.json")){
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
};

#endif