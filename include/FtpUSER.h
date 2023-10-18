#pragma once

#include <FtpTask.h>


class FtpUSER: public FtpTask {
public:
    void parse(const std::string& cmd, const std::string& arg) override;
};