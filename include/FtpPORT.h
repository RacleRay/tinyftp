#pragma once

#include <string>

#include <FtpTask.h>


class FtpPORT: public FtpTask {
public:
    void parse(const std::string& cmd, const std::string& msg) override;
};
