#include <DebugTool.h>
#include <FtpUSER.h>


void FtpUSER::parse(const std::string& cmd, const std::string& msg) {
    TESTOUT("CMD FtpUSER::parse : " << cmd << " " << msg);
    // Just simplify the logging procedure
    response_cmd("230 Logged in.\r\n");
}