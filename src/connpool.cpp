#include "connpool.h"

ConnPool* ConnPool::getConnPool() {
    static ConnPool pool;
    return &pool;
}

bool ConnPool::loadConfigFile_() {
    std::filesystem::path curFilePath = std::filesystem::current_path();
    std::string configPath = (curFilePath / "mysql.ini").string();
    LOG("Expected MySQL config path: " + configPath);
    FILE* fd = fopen(configPath.c_str(), "r");
    if (fd == nullptr) {
        LOG("MYSQL init file is not exist!");
        return false;
    }

    while (!feof(fd)) {
        char line[1024] = { 0 };
        fgets(line, 1024, fd); // get single line
        string str = line;
        int idx = str.find("=", 0);
        if (idx == -1) {
            // "=" not found
            continue;
        }
        string key = str.substr(0, idx);
        int endidx = str.find("\n", idx + 1);
        string value = str.substr(idx + 1, endidx - idx - 1);
        if (key == "ip") {
            ip_ = value;
        } else if (key == "port") {
            port_ = stoi(value.c_str());
        } else if (key == "username") {
            username_ = value;
        } else if (key == "dbname") {
            dbname_ = value;
        } else if (key == "password") {
            password_ = value;
        } else if (key == "initSize") {
            initSize_ = atoi(value.c_str());
        } else if (key == "maxSize") {
            maxSize_ = atoi(value.c_str());
        } else if (key == "maxIdleTime") {
            maxIdleTime_ = atoi(value.c_str());
        } else if (key == "getConnTimeout") {
            getConnTimeout_ = atoi(value.c_str());
        }
    }
    fclose(fd);
    return true;
}

ConnPool::ConnPool() : connCnt_(0) {
    if (!loadConfigFile_()) {
        // load config failed
        return;
    }

    LOG("Your sql init config: " + ip_ + " " + to_string(port_) + " " + username_ + " " + password_ + " "
        + dbname_ + " " + to_string(initSize_) + " " + to_string(maxSize_) + " " + to_string(maxIdleTime_)
        + " " + to_string(getConnTimeout_));

    for (int i = 0; i < initSize_; i++) {
        Conn* p = new Conn();
        p->connect(ip_, port_, username_, password_, dbname_);
        p->refreshIdleTime();
        connQue_.push(p);
        connCnt_++;
    }

    thread producer(std::bind(&ConnPool::produceConnTask_, this));
    producer.detach();

    thread scanner(std::bind(&ConnPool::scanConnTask_, this));
    scanner.detach();
}

void ConnPool::produceConnTask_() {
    unique_lock<mutex> lock(queMutex_);
    while (true) {
        while (!connQue_.empty()) {
            cv_.wait(lock);
        }

        if (connCnt_ < maxSize_) {
            Conn* conn = new Conn();
            conn->connect(ip_, port_, username_, password_, dbname_);
            conn->refreshIdleTime();
            connQue_.push(conn);
            connCnt_++;
        }
        cv_.notify_all();
    }
}

shared_ptr<Conn> ConnPool::getConn() {
    unique_lock<mutex> lock(queMutex_);
    while (connQue_.empty()) {
        if (std::cv_status::timeout == cv_.wait_for(lock, chrono::milliseconds(getConnTimeout_))) {
            if (connQue_.empty()) {
                LOG("Get MySQL connection timeout...");
                return nullptr;
            }
        }
    }
    /* shared_ptr (for connection auto release) with custom deleter implemented using lambda function */
    shared_ptr<Conn> sp(connQue_.front(), [&](Conn* pconn){
        unique_lock<mutex> lock(queMutex_);
        pconn->refreshIdleTime();
        connQue_.push(pconn);
    });  
    connQue_.pop();
    if (connQue_.empty()) {
        cv_.notify_all();
    }
    return sp;
}

void ConnPool::scanConnTask_() {
    while (true) {
        this_thread::sleep_for(chrono::seconds(maxIdleTime_));

        unique_lock<mutex> lock(queMutex_);
        while(connQue_.empty()) {
            cv_.wait(lock);
        }

        while(connCnt_ > initSize_) {
            Conn* p = connQue_.front();
            if (p->getIdleTime() > maxIdleTime_) {
                connQue_.pop();
                connCnt_--;
                delete p;
            } else {
                break;
            }
        }
    }
}