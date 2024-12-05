#ifndef LOG_H
#define LOG_H
#include <iostream>

#define LOG(str) \
    std::cout << __FILE__ << ":" << __LINE__ << " " << __TIMESTAMP__ \
    << " : " << str << std::endl;

#endif