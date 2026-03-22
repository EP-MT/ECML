#include <iostream>
#include <string>
#include <vector>
#include "httplib.h"
#include "windows.h"
#define CPPHTTPLIB_OPENSSL_SUPPORT

const std::string delimArray[5] = {".", "/", "www.", "http://", "https://"};
std::vector<std::string> output;

std::vector<std::string> parseWeb()
{
    std::string input, domain, path;
    std::cin >> input;

    int wwwpos = input.find(delimArray[2]);
    if (wwwpos != std::string::npos)
    {
        input = input.substr(wwwpos + delimArray[2].length());
    }

    else if ((wwwpos = input.find(delimArray[3])) != std::string::npos)
    {
        input = input.substr(wwwpos + delimArray[3].length());
    }

    else if ((wwwpos = input.find(delimArray[4])) != std::string::npos)
    {
        input = input.substr(wwwpos + delimArray[4].length());
    }

    int pos = input.find(delimArray[1]);
    path = input.substr(pos);
    input = input.substr(0, pos);

    int dompos = input.find(delimArray[0]);
    domain = input.substr(dompos);

    std::cout << "CLI path:" + input << std::endl
              << "Domain:" + domain << std::endl
              << "Get path:" + path << std::endl;

    return {input, domain, path};
}

void Request()
{
    output = parseWeb();

    httplib::SSLCLient client(output[0]);
    client.enable_server_certificate_verification(false);

    auto log_error = [](const httplib::Error &err)
    {
        std::cerr << httplib::to_string(err) << std::endl;
    };

    if (auto response = client.Get(output[2]))
    {
        if (response->status == httplib::StatusCode::OK_200)
        {
            std::cout << response->body << std::endl;
        }
    }

    else
    {
        log_error(response.error());
    }
}

int main()
{
    parseWeb();
}