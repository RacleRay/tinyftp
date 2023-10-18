#include <cstring>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/util.h>
#include <string>

#include <DebugTool.h>
#include <ServerCMD.h>


// 注册 client 发送的 task
void CmdServer::regist(const std::string &cmd, FtpTask* task) {
    TESTOUT("ServerCMD::regist : register cmd : " << cmd);

    if (task == nullptr) {
        std::cout << "ServerCMD::regist : task is nullptr" << std::endl;
        return;
    }
    if (cmd.empty()) {
        std::cout << "ServerCMD::regist : cmd is empty" << std::endl;
        return;
    }

    // 检测重复命令
    if (cmd2tasks.find(cmd) != cmd2tasks.end()) {
        std::cout << "ServerCMD::regist : " << cmd << " is exist" << std::endl;
        return;
    }

    TESTOUT("ServerCMD::regist : register success.");
    
    task->evt_base = evt_base;  // TODO 
    task->cmd_server = this;
    
    cmd2tasks[cmd] = task;

}


void CmdServer::event(bufferevent *bev, short event) {
    TESTOUT("ServerCMD::event : event : " << event);
    if (event & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
        delete this;
    }
}


// 解析 client 发送的命令
void CmdServer::read(bufferevent *bev) {
    TESTOUT("ServerCMD::read : read");

    char buf[BUF_SIZE] =  {0};
    while (true) {
        size_t rlen = bufferevent_read(bev, buf, BUF_SIZE);
        if (rlen <= 0) { break; }

        std::cout << "Received CMD: " << buf << std::endl;

        std::string type;
        for (size_t i = 0; i < rlen; i++) {
            if (buf[i] == ' ' || buf[i] == '\r') { break; }
            type += buf[i];
        }
        std::cout << "CMD type: " << type << std::endl;

        if (cmd2tasks.find(type) != cmd2tasks.end()) {
            TESTOUT("ServerCMD::read : begin to parse cmd.");
            FtpTask *task = cmd2tasks[type];
            task->parse(type, buf); 
            TESTOUT("Current directory is: " << cur_dir);
        } else {
            std::cout << "ServerCMD::read : client cmd not found." << std::endl;
            response_cmd("200 OK\r\n");
        }
    }
}


bool CmdServer::init() {
    TESTOUT("ServerCMD::init : init");

    // 命令通道
    bev = bufferevent_socket_new(evt_base, conn_sock, BEV_OPT_CLOSE_ON_FREE);
    if (!bev) {
        delete this;
        return false;
    }

    timeval t = {300, 0};  // 300s
    bufferevent_set_timeouts(bev, &t, nullptr);

    std::string msg = "220 Welcome to FTP server.\r\n";
    bufferevent_write(bev, msg.c_str(), msg.size());

    this->cmd_server = this;
    
    set_callback();

    return true;
}


CmdServer::~CmdServer() {
    closeDataPort();
    // TODO: 清理Task？
}