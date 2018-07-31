/*
 * CommandLineArgumentParser.cpp
 *
 *  Created on: July 31, 2018
 *      Author: nirl
 */
#include <utility>
#include "CommandLineArgumentParser.h"
using namespace boost::program_options;

#include <iostream>
using namespace std;

/* Auxiliary functions for checking input for validity. */

/* Function used to check that 'opt1' and 'opt2' are not specified
   at the same time. */
void CommandLineArgumentParser::conflicting_options(const variables_map& vm,
                         const char* opt1, const char* opt2)
{
    if (vm.count(opt1) && !vm[opt1].defaulted()
        && vm.count(opt2) && !vm[opt2].defaulted())
        throw logic_error(string("Conflicting options '")
                          + opt1 + "' and '" + opt2 + "'.");
}

/* Function used to check that of 'for_what' is specified, then
   'required_option' is specified too. */
void CommandLineArgumentParser::option_dependency(const variables_map& vm,
                        const char* for_what, const char* required_option)
{
    if (vm.count(for_what) && !vm[for_what].defaulted())
        if (vm.count(required_option) == 0 || vm[required_option].defaulted())
            throw logic_error(string("Option '") + for_what
                              + "' requires option '" + required_option + "'.");
}

int CommandLineArgumentParser::validate(int argc, char* argv[])
{
    try {

        string depends = "deps_file";
        string sources = "src_file";
        string root = ".";

        options_description desc("Allowed options");
        desc.add_options()
        // First parameter describes option name/short name
        // The second is parameter to option
        // The third is description
        ("method,m", value(&_method)->default_value("epoll"), "select, poll, epoll")
        ("help,h", "print usage message")
       ;

        store(parse_command_line(argc, argv, desc), _vm);

        if (_vm.count("help") || argc<2) {
            cout << desc << "\n";
            return 1;
        }
        notify(_vm);

        //conflicting_options(_vm, "local", "server_ip");

        /*
        option_dependency(_vm, "server_ip", "server_port");
        option_dependency(_vm, "protocol_type", "server_port");
        option_dependency(_vm, "cam_protocol_type", "cam_server_port");
        option_dependency(_vm, "protocol_type", "message_type");
         */


    }
    catch(exception& e) {
        cerr << e.what() << "\n";
        throw e;
    }
    return 0;
}

string CommandLineArgumentParser::get_value(string &&field)
{

	string s;
	try
	{
	 	s = _vm[field].as<string>();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "failed parsing:"<<field<<", error:"<< ex.what() << '\n';
		s = InvalidEntry;
	}
	return std::move(s);

}

int CommandLineArgumentParser::get_int_value(string &&field)
{
	int ret;
	try
	{
		ret = _vm[field].as<int>();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "failed parsing:"<<field<<", error:"<< ex.what() << '\n';
		ret = -1;
	}
	return ret;
}

bool CommandLineArgumentParser::get_bool_value(string &&field)
{
	bool ret;
	try
	{
		ret = _vm[field].as<bool>();
	}
	catch (const std::exception& ex)
	{
		std::cerr << "failed parsing:"<<field<<", error:"<< ex.what() << '\n';
		ret = -1;
	}
	return ret;
}

void CommandLineArgumentParser::dump()
{
	std::cout<<"method:"<<get_method()<<std::endl;
}


CommandLineArgumentParser::CommandLineArgumentParser() {}

CommandLineArgumentParser::~CommandLineArgumentParser() {
	// TODO Auto-generated destructor stub
}

