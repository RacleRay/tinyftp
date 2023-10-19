#include <event2/bufferevent.h>
#include <event2/event.h>
#include <iostream>
#include <string>

#include <DebugTool.h>
#include <FtpSTOR.h>


void FtpSTOR::parse(const std::string& cmd, const std::string& msg) {
    TESTOUT("FtpSTOR::parse");

    // get filename
    int pos = msg.rfind(' ') + 1;
    std::string filename = msg.substr(pos, msg.size() - pos - 2);
    std::string path = cmd_server->root_dir + cmd_server->cur_dir + filename;
    TESTOUT("FtpSTOR::parse file path: " << path);

    // open file
    fp = fopen(path.c_str(), "wb");
    if (fp) {
        connectDataPort();
        response_cmd("125 file ok");
        bufferevent_trigger(bev, EV_READ, 0);
    } else {
        response_cmd("450 file open failed.");
    }
}


void FtpSTOR::read(bufferevent *bev) {
    TESTOUT("FtpSTOR::read");

    if (!fp) { return; }

    while (true) {
        size_t rlen = bufferevent_read(bev, buf, sizeof(buf));
        if (rlen <= 0) {
            return;
        }
        fwrite(buf, 1, rlen, fp); // write to file fp
    }
}


void FtpSTOR::event(bufferevent *bev, short event) {
    TESTOUT("FtpSTOR::event");

    if (event & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT)) {
        closeDataPort();
        response_cmd("226 transfer complete.");
    } else if (event & BEV_EVENT_CONNECTED) {
        std::cout << "FtpSTOR::event : connected" << std::endl;
    }
}