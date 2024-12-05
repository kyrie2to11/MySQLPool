---
title: SQL Pool Project
date: 2024-12-03 14:49:09
tags:
---

## SQL Pool Project 背景

- 为了提高 MySQL 数据库的访存瓶颈，在大量 SQL connection 并发的情况下，重复的**TCP三次握手 -> MySQL Server 连接认证 -> MySQL Server 关闭连接回收资源 -> TCP 四次挥手**消耗大量时间，增加连接池可以减少此部分耗时

## 技术点

1. MySQL 数据库编程
2. 单例模式
3. queue 队列容器
4. c++11 多线程编程、线程互斥、线程同步通信和 unique_lock
   1. 进程与线程的区别和举例
        - 资源占用
           1. 进程是资源分配的基本单位。每个进程有独立的地址空间、代码段、数据段、堆和栈。如打开一个文本编辑器和浏览器，这是两个不同的进程，资源独立
           2. 线程是进程内的执行单元，共享所属进程的资源。如一个文本编辑器进程中，可能有一个线程负责接收用户键盘输入，另一个线程负责后台保存文档。这些线程共享文本编辑器进程的代码段、数据段等。
        - 调度开销
            1. 进程切换开销大，线程切换开销较小
   2. 进程通信方式
        - 管道(Pipe): 半双工通信（同一时刻单向信息流动），用于具有亲缘关系的进程通信
        - 消息队列(Message Queue): 相较管道克服了管道只能在具有亲缘关系的进程间通信的限制
        - 共享内存(Shared Memory)
        - 信号量(Semaphore): 相较线程通信条件变量区别在于有状态，实际上是一个计数器，可正可负，用于标识共享资源的数目或阻塞的进程数目
   3. 线程通信方式
        - 互斥锁(Mutex)
        - 条件变量(Condition_Variable): 无状态
   4. 条件变量搭配 uniqe_lock 而不能搭配 lock_guard 原因：
        - Condition_Variable cv.wait(mtx) 在等待信号量通知时会先把锁释放掉，unique_lock 允许灵活的释放和获取锁。而 lock_guard 是 RAII 类型的模板类，lock_guard 对象在创建时获取互斥量的锁，析构时自动释放锁，不提供手动解锁接口
5. 基于 CAS 的原子整形: (Compare and Switch) CAS
6. 智能指针 shared_ptr
7. lambda 表达式
8. 生产者-消费者线程模型

## 功能点

1. connection_pool getConnPool(): 单例模式
2. 空闲 mysql 连接维护在一个线程安全的 connection_queue 中：加锁互斥访问
3. 消费者：get_connection: 从 connection_pool 获取连接，需要处理获取连接超时的情况
4. 生产者：如果 connection_queue 为空，需要动态创建 connection,上限 maxSize
5. connection_queue 中空闲时长超过 maxIdleTime 的需要被释放掉，只保留初始 initSize 个 connection 即可

## 压力测试

---

|数据量|不用 SQL Pool|使用 SQL Pool|
|-----|-------------|-----------|
|1000|单线程：50768ms|单线程：10407.6ms 四线程：1980.9ms|
|5000|单线程：244466ms|单线程：21791.8ms 四线程：7504.81ms|
|10000|单线程：493808ms|单线程：37665.3ms 四线程：14478.5ms|

---
