// g++ -g3 -std=c++1z get_my_ip.cpp

#include <string>   //std::string
#include <memory>   //std::shared_ptr
#include <array>    //std::array since c++11
using namespace std;
const std::string bash_command = "ifconfig | grep -Eo 'inet (addr:)?([0-9]*\\.){3}[0-9]*' | grep -Eo '([0-9]*\\.){3}[0-9]*' | grep -v '127.0.0.1'";

std::string get_local_ip()
{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(bash_command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    result.erase(result.find_last_not_of("\n\r\t")+1);
    return std::move(result);
}

int  main()
{
    printf("my local ip is :%s\n",get_local_ip().c_str());
}

//posible output 
//my local ip is :192.168.190.167
