//
// Created by anmol on 3/12/22.
//
#include <iostream>

#define RED "\033[31m"
#define RESET "\033[0m"

class Logger {
public :
    void print_error(const char *msg) {
        std::cout << RED << "ERROR : " << RESET << msg << std::endl;
    }

    void print_info(const char *msg) {
        std::cout << "INFO : " << msg << std::endl;
    }
};