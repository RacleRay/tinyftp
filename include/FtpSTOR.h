#pragma once

#include <Config.h>
#include <FtpTask.h>


// 接收上传数据
class FtpSTOR: public FtpTask {
public:
    void parse(const std::string &cmd, const std::string &msg) override;
    void read(bufferevent *bev) override;
    void event(bufferevent *bev, short event) override;

private:
    char buf[DATA_BUF_SIZE] = {0};
};