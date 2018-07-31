#include "epoll.h"
#include "poll.h"
#include "select.h"
#include <iostream>
#include "CommandLineArgumentParser.h" 

int main(int argc, char* argv[]){
    CommandLineArgumentParser CLAP;
    try
    {
        if (CLAP.validate(argc, (char **)argv))
        {
            return 0;
        }
    
    }
    catch (const std::exception& e)//(std::exception &ex)
    {
        std::cout<<"illegal arguments"<<std::endl;
        return EINVAL;
    }
    std::cout<<CLAP.get_method()<<std::endl;
    if (CLAP.get_method() == "select")
    {
        std::cout<<"running select"<<std::endl;
        select_main();
    }
    else if (CLAP.get_method() == "poll")
    {
        std::cout<<"running poll"<<std::endl;
        poll_main();
    }
    else if (CLAP.get_method() == "epoll")
    {
        std::cout<<"running epoll"<<std::endl;
        epoll_main();
    }

}
