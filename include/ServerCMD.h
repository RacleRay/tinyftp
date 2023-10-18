#pragma once

#include <map>
#include <memory>

#include <FtpTask.h>


#define BUF_SIZE 4096


class CmdServer: public FtpTask {
public:
    bool init() override;

    void event(bufferevent *be, short events) override;
    void read(bufferevent *be) override;

    void regist(const std::string &cmd, FtpTask *task);

    CmdServer();
    ~CmdServer();

private:
    std::map<std::string, FtpTask*> cmd2tasks;  // TODO: 
    std::map<FtpTask*, int> tasks_del;
};