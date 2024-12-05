#include "./src/conn.h"
#include "./src/connpool.h"
#include <chrono>
using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();
    /* single thread test */
    // for (int i = 0; i < 1000; ++i) {
    //     /* don't use sql connection pool */
    //     // Conn conn;
    //     // char sql[1024] = { 0 };
    //     // sprintf(sql, "insert into user(username, password) values('%s','%s')", "zephyrus", "qq105311");
    //     // conn.connect("127.0.0.1", 3306, "root", "qq105311", "testdb");
    //     // conn.update(sql);

    //     /* use sql connection pool */
    //     ConnPool* cp = ConnPool::getConnPool();
    //     shared_ptr<Conn> sp = cp->getConn();
    //     char sql[1024] = { 0 };
    //     sprintf(sql, "insert into user(username, password) values('%s','%s')", "zephyrus", "qq105311");
    //     sp->update(sql);
    // }


    /* multi thread test */
    thread t1([](){
        /* use sql connection pool */
        ConnPool* cp = ConnPool::getConnPool();
        for (int i = 0; i < 2500; ++i) {
            /* don't use sql connection pool */
            // Conn conn;
            // char sql[1024] = { 0 };
            // sprintf(sql, "insert into user(username, password) values('%s','%s')", "zephyrus", "qq105311");
            // conn.connect("127.0.0.1", 3306, "root", "qq105311", "testdb");
            // conn.update(sql);

            /* use sql connection pool */
            shared_ptr<Conn> sp = cp->getConn();
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(username, password) values('%s','%s')", "zephyrus", "qq105311");
            sp->update(sql);
        }
    });
    thread t2([](){
        /* use sql connection pool */
        ConnPool* cp = ConnPool::getConnPool();
        for (int i = 0; i < 2500; ++i) {
            /* don't use sql connection pool */
            // Conn conn;
            // char sql[1024] = { 0 };
            // sprintf(sql, "insert into user(username, password) values('%s','%s')", "zephyrus", "qq105311");
            // conn.connect("127.0.0.1", 3306, "root", "qq105311", "testdb");
            // conn.update(sql);

            /* use sql connection pool */
            shared_ptr<Conn> sp = cp->getConn();
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(username, password) values('%s','%s')", "zephyrus", "qq105311");
            sp->update(sql);
        }
    });
    thread t3([](){
        /* use sql connection pool */
        ConnPool* cp = ConnPool::getConnPool();
        for (int i = 0; i < 2500; ++i) {
            /* don't use sql connection pool */
            // Conn conn;
            // char sql[1024] = { 0 };
            // sprintf(sql, "insert into user(username, password) values('%s','%s')", "zephyrus", "qq105311");
            // conn.connect("127.0.0.1", 3306, "root", "qq105311", "testdb");
            // conn.update(sql);

            /* use sql connection pool */
            shared_ptr<Conn> sp = cp->getConn();
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(username, password) values('%s','%s')", "zephyrus", "qq105311");
            sp->update(sql);
        }
    });
    thread t4([](){
        /* use sql connection pool */
        ConnPool* cp = ConnPool::getConnPool();
        for (int i = 0; i < 2500; ++i) {
            /* don't use sql connection pool */
            // Conn conn;
            // char sql[1024] = { 0 };
            // sprintf(sql, "insert into user(username, password) values('%s','%s')", "zephyrus", "qq105311");
            // conn.connect("127.0.0.1", 3306, "root", "qq105311", "testdb");
            // conn.update(sql);

            /* use sql connection pool */
            shared_ptr<Conn> sp = cp->getConn();
            char sql[1024] = { 0 };
            sprintf(sql, "insert into user(username, password) values('%s','%s')", "zephyrus", "qq105311");
            sp->update(sql);
        }
    });

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;  // 计算时间间隔并转换为毫秒单位
    cout << elapsed.count() << "ms" << endl;

    return 0;
}