#pragma once

#include <Config.h>
#include <FtpTask.h>

// 向client传输下载数据
class FtpRETR: public FtpTask {
public:
    void parse(const std::string& cmd, const std::string& msg) override;
    void event(bufferevent *bev, short event) override;
    void write(bufferevent *bev) override;

    bool init() override { return true; }

private:
    char buf[DATA_BUF_SIZE] = {0};
};