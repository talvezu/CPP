/*
 * CommandLineArgumentParser.h
 *
 *  Created on: Jul 31, 2018
 *      Author: nirl
 */

#ifndef COMMANDLINEARGUMENTPARSER_H_
#define COMMANDLINEARGUMENTPARSER_H_
#include <string>
#include <boost/program_options.hpp>
class CommandLineArgumentParser {

    std::string _method;

    boost::program_options::variables_map _vm;
    void option_dependency(const boost::program_options::variables_map& vm,
                           const char* for_what,
    			   const char* required_option);
    
    void conflicting_options(const boost::program_options::variables_map& vm,
    		             const char* opt1,
    			     const char* opt2);
    
    std::string get_value(std::string &&field);
    bool get_bool_value(std::string &&field);
    int get_int_value(std::string &&field);

public:
    int validate(int argc, char* argv[]);
    std::string InvalidEntry{"invalid"};
    void dump();
    std::string get_method(){return get_value("method");}
    
    CommandLineArgumentParser();
    
    CommandLineArgumentParser(const CommandLineArgumentParser &rhs)=delete;
    
    virtual ~CommandLineArgumentParser();
};

#endif /* COMMANDLINEARGUMENTPARSER_H_ */
