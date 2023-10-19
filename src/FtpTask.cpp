#include <cstring>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>

#include <DebugTool.h>
#include <FtpTask.h>


void FtpTask::connectDataPort() {
    TESTOUT("FtpTask::connectDataPort : connecting to data transition port.")

    // connect 之前，需要先通过 PORT 命令，传输数据通道的目标地址
    // cmd_task 即是在  PORT 命令响应时，添加到 cmd_task 中的
    if (cmd_server->ipaddr.empty() || cmd_server->port <= 0 || cmd_server->port > 65535
        || cmd_server->evt_base == nullptr) {
       std::cout << "FtpTask::connectDataPort : connect failed." << std::endl;
       return;
    }

    if (bev) {
        bufferevent_free(bev);
        bev = nullptr;
    }

    bev = bufferevent_socket_new(cmd_server->evt_base, -1, BEV_OPT_CLOSE_ON_FREE);

    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(cmd_server->port);
    evutil_inet_pton(AF_INET, cmd_server->ipaddr.c_str(), &sin.sin_addr);

    // 设定 bufferevent 的回调函数
    set_callback();
    
    // 添加读60s超时，写没有超时
    timeval t = {60, 0};
    bufferevent_set_timeouts(bev, &t, nullptr);

    bufferevent_socket_connect(bev, (sockaddr*)&sin, sizeof(sin));
}


void FtpTask::closeDataPort() {
    if (bev) {
        bufferevent_free(bev);
        bev = nullptr;
    }
    if (fp) {
        (void)fclose(fp);
        fp = nullptr;
    }
}


void FtpTask::send(const std::string &msg) const {
    // TESTOUT("FtpTask::send : send msg to ftp client.")
    send(msg.c_str(), msg.size());
}


void FtpTask::send(const char *msg, size_t len) const {
    TESTOUT("FtpTask::send : send msg to ftp client.");

    std::cout << msg;

    if (len == 0) { return; }

    if (bev) {
        bufferevent_write(bev, msg, len);
    }
}


void FtpTask::response_cmd(std::string msg) {
    TESTOUT("FtpTask::response_cmd : ftp response cmd to client.");

    if (!cmd_server || !cmd_server->evt_base) {
        return;
    }

    std::cout << "Response of client cmd : " << msg << std::endl << std::flush;
    
    // fix format.
    if (msg[msg.size() - 1] != '\n') {
        msg += "\r\n";
    }

    bufferevent_write(cmd_server->bev, msg.c_str(), msg.size());

    // bufferevent_flush(cmd_server->bev, EV_WRITE, BEV_FLUSH);
}


void FtpTask::set_callback() {
    bufferevent_setcb(bev, read_callback, write_callback, event_callback, this);
    // watch event
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}


// read_callback 是 bufferevent_setcb 的统一接口；不同的Task实现 read 的重写
void FtpTask::read_callback(struct bufferevent *bev, void *arg) {
    FtpTask *t = (FtpTask*)arg;
    t->read(bev);
}


void FtpTask::write_callback(struct bufferevent *bev, void *arg) {
    FtpTask *t = (FtpTask*)arg;
    t->write(bev);
}


void FtpTask::event_callback(struct bufferevent *bev, short event, void *arg) {
    FtpTask *t = (FtpTask*)arg;
    t->event(bev, event);
}


FtpTask::~FtpTask() {
    closeDataPort();
}