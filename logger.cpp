//
// Created by anmol on 3/12/22.
//
#include <iostream>

#define RED "\033[31m"
#define RESET "\033[0m"

class Logger {
public :
    void print_error(const char *msg){
        std::cout<<RED<<"Error : "<<RESET<<msg<<std::endl;
    }
};