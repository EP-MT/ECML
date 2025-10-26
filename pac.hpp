#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <iostream>
#include <filesystem>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include "httplib.h"
#include "windows.h"

namespace fs = std::filesystem;
using def = std::function<void()>;


std::string GetArchitecture(){
    SYSTEM_INFO si;
    GetNativeSystemInfo(&si);
    
    switch(si.wProcessorArchitecture){
        case PROCESSOR_ARCHITECTURE_AMD64:
        return "AMD64";
        
        case PROCESSOR_ARCHITECTURE_ARM64:
        return "ARM64";
        
        case PROCESSOR_ARCHITECTURE_INTEL:
        return "x86 (32-bit)";
        
        case PROCESSOR_ARCHITECTURE_ARM:
        return "ARM (32-bit)";
        
        case PROCESSOR_ARCHITECTURE_IA64:
        return "Intel Itanium (IA-64)";
        
        default:
        return "Unkown";
    }
}

std::string Architecture = GetArchitecture();

class req
{
    private:
        const std::string delimArray[5] = {".", "/", "www.", "http://", "https://"};

    public:
        std::vector<std::string> parseWeb(){
            std::string input, domain, path;
            std::cin >> input;
            
            int wwwpos = input.find(delimArray[2]);
            if(wwwpos != std::string::npos){
                input = input.substr(wwwpos + delimArray[2].length());
            }
            
            else if((wwwpos = input.find(delimArray[3])) != std::string::npos){
                input = input.substr(wwwpos + delimArray[3].length());
            }

            else if((wwwpos = input.find(delimArray[4])) != std::string::npos){
                input= input.substr(wwwpos + delimArray[4].length());
            }


            int pos = input.find(delimArray[1]);
            path = input.substr(pos);
            input = input.substr(0, pos);

            int dompos = input.find(delimArray[0]);
            domain = input.substr(dompos);

            //std::cout << "CLI path:" + input << std::endl << "Domain:" + domain << std::endl << "Get path:" + path << std::endl;

            return {input, domain, path};
        }

        void Request(){
            std::vector<std::string> output = parseWeb();
            
            for(auto i : output){
                std::cout << i << std::endl;
            }

            httplib::SSLClient client(output[0]);
            client.enable_server_certificate_verification(false);

            auto log_error = [](const httplib::Error &err) {
                std::cerr << httplib::to_string(err) << std::endl;
            };

            if(auto response = client.Get(output[2])){
                if(response->status == httplib::StatusCode::OK_200){
                    std::cout << response->body << std::endl;
                }
            }

            else{
                log_error(response.error());
                }
        }
};