#include <iostream>
#include <vector>

#include <DebugTool.h>
#include <FtpPORT.h>


// Parse format: 
// PORT 127,0,0,1,70,96\r\n
// PORT n1,n2,n3,n4,n5,n6\r\n
//      port = n5 * 256 + n6
void FtpPORT::parse(const std::string& cmd, const std::string& msg) {
    TESTOUT("CMD FtpPORT::parse : " << cmd << " " << msg);

    std::vector<std::string> tokens;
    std::string tmp;
    for (int i = 5; i < msg.size(); i++) {
        if (msg[i] == ',' || msg[i] == '\r') {
            tokens.push_back(tmp);
            tmp.clear();
            continue;
        }
        tmp += msg[i];
    }

    if (tokens.size() != 6) {
        response_cmd("501 Syntax error: Invalid PORT format");
        return;
    }

    ipaddr = tokens[0] + "." + tokens[1] + "." + tokens[2] + "." + tokens[3];
    port = std::stoi(tokens[4]) * 256 + std::stoi(tokens[5]);

    // 通知命令通道服务端，客户端开启的数据通道端口地址
    cmd_server->ipaddr = ipaddr;
    cmd_server->port = port;

    TESTOUT("CMD FtpPORT::parse : " << ipaddr << " " << port);
    response_cmd("200 PORT command success");
}