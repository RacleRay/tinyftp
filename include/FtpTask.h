#pragma once

#include <event2/bufferevent.h>
#include <string>

#include <Task.h>

/**
 * @brief Important for understanding FTP server working logic.
 * 
 */

class FtpTask: public Task {
public:
    std::string cur_dir = "/";
    std::string root_dir = ".";

    std::string ipaddr;
    int port;

    FtpTask* cmd_server = nullptr;  // 解析客户端请求得到的任务

    // 数据传输建立的连接
    void connectDataPort();
    void closeDataPort();

    void send(const std::string& msg) const;
    void send(const char* msg, size_t len) const;

    void response_cmd(std::string msg);

    void set_callback();
    
    bool init() override { return true; };

    virtual void parse(const std::string& type, const std::string& msg) {};

    virtual void event(bufferevent* bev, short event) {};
    virtual void read(bufferevent* bev) {};
    virtual void write(bufferevent* bev) {};

    ~FtpTask();

protected:
    static void event_callback(bufferevent* bev, short event,void* arg);
    static void read_callback(bufferevent* bev, void* arg);
    static void write_callback(bufferevent* bev, void* arg);

    bufferevent *bev = nullptr;
    FILE *fp = nullptr;
};