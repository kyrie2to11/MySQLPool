/* mysql connection */
#ifndef CONN_H
#define CONN_H

#include <mysql/mysql.h>
#include <string>
#include <ctime>

#include "log.h"
using namespace std;

class Conn {
public:
    Conn();
    ~Conn();
    bool connect(string ip,
                    unsigned short port,
                    string username,
                    string password,
                    string dbname);
    bool update(string sql); // insert/delete/update
    MYSQL_RES* query(string sql); // query
    clock_t refreshIdleTime() { idleTime_ = clock(); };
    clock_t getIdleTime() { return (clock() - idleTime_) / CLOCKS_PER_SEC; };

private:
    MYSQL* conn_;
    clock_t idleTime_; // entry time point into idle
};

#endif