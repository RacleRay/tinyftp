#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <string>

#include <DebugTool.h>
#include <FtpRETR.h>


void FtpRETR::parse(const std::string& cmd, const std::string& msg) {
    TESTOUT("FtpRETR::parse : " << cmd << " " << msg);

    // get filename
    int pos = msg.rfind(' ') + 1;
    std::string filename = msg.substr(pos, msg.size() - pos - 2);
    std::string path = cmd_server->root_dir + cmd_server->cur_dir + filename;
    TESTOUT("FtpRETR::parse file path: " << path);

    // open file
    fp = fopen(path.c_str(), "rb");
    if (fp) {
        connectDataPort();
        response_cmd("150 file ok");
        // invoke write_callback
        bufferevent_trigger(bev, EV_WRITE, 0);
    } else {
        response_cmd("450 file not found");
    }
}


void FtpRETR::write(bufferevent* bev) {
    TESTOUT("FtpRETR::write");

    if (!fp) { return; }
    
    size_t rlen = fread(buf, 1, sizeof(buf), fp);
    if (rlen <= 0) {
        response_cmd("226 transfer complete");
        closeDataPort();
        return;
    }

    TESTOUT("FtpRETR::write : " << buf);
    send(buf, sizeof(buf));
}


void FtpRETR::event(bufferevent* bev, short event) {
    TESTOUT("FtpRETR::event");

    if (event & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
        closeDataPort();
    } else if (event & BEV_EVENT_CONNECTED) {
        std::cout << "FtpRETR::event : connected" << std::endl;
    }
}