#pragma once 

#include <string>

#include <Config.h>
#include <FtpTask.h>


class FtpLIST: public FtpTask {
public:
    void parse(const std::string& cmd, const std::string& msg) override;
    void event(bufferevent* bev, short event) override;
    void write(bufferevent* bev) override;

private:
    static std::string getListData(const std::string& path);
};