#include <event2/bufferevent.h>
#include <event2/event.h>
#include <string>

#include <DebugTool.h>
#include <FtpLIST.h>


std::string FtpLIST::getListData(const std::string& path) {
    std::string data;
    std::string cmd = "ls -l ";
    cmd += path;

    FILE* fp = popen(cmd.c_str(), "r");
    if (!fp) { return data; }

    char buf[BUF_SIZE] = {0};
    while (true) {
        size_t rlen = fread(buf, 1, BUF_SIZE, fp);
        if (rlen <= 0) { break; }
        buf[rlen] = 0;
        data += buf;
    }

    pclose(fp);
    return data;
}


void FtpLIST::parse(const std::string& cmd, const std::string& msg) {
    TESTOUT("CMD FtpLIST::parse : " << cmd << " " << msg);

    std::string response_msg;

    if (cmd == "PWD") {
        response_msg = "257 \"" + msg + "\" is current directory.\r\n";
        response_cmd(response_msg);
    } else if (cmd == "LIST") {
        // 命令通道回复状态消息，数据通道发送LIST目录内容
        std::string path = cmd_server->root_dir + cmd_server->cur_dir;
        TESTOUT("LIST path : " << path);

        std::string data = getListData(path);

        connectDataPort();
        response_cmd("150 Sending the directory list.\r\n");

        send(data);
    } else if (cmd == "CWD") {  // 切换目录
        int path_pos = msg.rfind(" "); 
        // remove \r\n
        std::string path = msg.substr(path_pos, msg.size() - path_pos - 2);

        if (path[0] == '/') {  // 绝对路径
            cmd_server->cur_dir = path;
        } else { // 相对路径
            if (cmd_server->cur_dir[cmd_server->cur_dir.size() - 1] != '/') {
                cmd_server->cur_dir += "/";
            } 
            cmd_server->cur_dir += path + "/";
        }

        if (cmd_server->cur_dir[cmd_server->cur_dir.size() - 1] != '/') {
            cmd_server->cur_dir += "/";
        }

        response_cmd("250 Directory changed successfully.\r\n");
    } else if (cmd == "CDUP") {  // 返回上层目录
        if (msg[4] == '\r') {  // 没有跟路径名时，默认返回 root 
            cmd_server->cur_dir = "/";
        } else {
            std::string path = cmd_server->cur_dir;

            // 找到上层目录的结束位置
            if (path[path.size() - 1] == '/') {
                path = path.substr(0, path.size() - 1);
            }
            int pos = path.rfind("/");
            path = path.substr(0, pos);
            
            cmd_server->cur_dir = path;
            if (cmd_server->cur_dir[cmd_server->cur_dir.size() - 1] != '/') {
                cmd_server->cur_dir += "/";
            }
        }

        response_cmd("250 Directory changed successfully.\r\n");
    }
}


void FtpLIST::event(bufferevent* bev, short event) {
    TESTOUT("FtpLIST::event : " << event);

    if (event & (BEV_EVENT_ERROR | BEV_EVENT_EOF | BEV_EVENT_TIMEOUT)) {
        closeDataPort();
    } else if (event & BEV_EVENT_CONNECTED) {
        std::cout << "FtpLIST::event : BEV_EVENT_CONNECTED" << std::endl;
    }
}


// TODO: write 的调用时机
void FtpLIST::write(bufferevent *bev) {
    TESTOUT("FtpLIST::write");

    response_cmd("226 LIST data transfered successfully.\r\n");

    closeDataPort();
}