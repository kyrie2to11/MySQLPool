#include "conn.h"

Conn::Conn() {
    conn_ = mysql_init(nullptr);
}

Conn::~Conn() {
    if (conn_ != nullptr) {
        mysql_close(conn_);
    }
}

bool Conn::connect(string ip,
                    unsigned short port,
                    string username,
                    string password,
                    string dbname) {
    MYSQL* p = mysql_real_connect(conn_, ip.c_str(), username.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
    return p != nullptr;
}

bool Conn::update(string sql) {
    if (mysql_query(conn_, sql.c_str())) {
        LOG("Update Failed: " + sql);
        return false;
    }
    return true;
}

MYSQL_RES* Conn::query(string sql) {
    if (mysql_query(conn_, sql.c_str())) {
        LOG("Query Failed: " + sql);
        return nullptr;
    }
    return mysql_use_result(conn_);
}

