#ifndef MYSQL_POOL_H
#define MYSQL_POOL_H

#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <thread>
#include <functional>
#include <filesystem>
#include <cstdio>
#include <chrono>

#include "log.h"
#include "conn.h"

class ConnPool {
public:
    static ConnPool* getConnPool();
    shared_ptr<Conn> getConn();

private:
    ConnPool();
    bool loadConfigFile_();
    void produceConnTask_();
    void scanConnTask_();

    /* Connection Queue */
    queue<Conn*> connQue_;
    int initSize_;
    int maxSize_;
    int maxIdleTime_; // s
    int getConnTimeout_; // ms
    atomic_int connCnt_;
    mutex queMutex_;
    condition_variable cv_;

    /* MYSQL */
    string ip_;
    unsigned short port_;
    string username_;
    string password_;
    string dbname_;
};

#endif