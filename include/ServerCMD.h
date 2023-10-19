#pragma once

#include <map>
#include <memory>
#include <vector>

#include <Config.h>
#include <FtpTask.h>


class CmdServer: public FtpTask {
public:
    bool init() override;

    void event(bufferevent *be, short events) override;
    void read(bufferevent *be) override;

    void regist(const std::string &cmd, FtpTask *task);

    CmdServer() = default;
    ~CmdServer();

private:
    std::map<std::string, FtpTask*> cmd2tasks;  // TODO: 
    std::vector<FtpTask*> tasks2delete;
};