
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/http.h>
#include <event2/keyvalq_struct.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <iostream>
#include <string>

#include <DebugTool.h>
#include <ThreadPool.h>

#define SERVER_PORT 21

inline void errmsg(const std::string &msg) {
    std::cout << msg << " failed." << std::endl;
}

void listen_callback(
    struct evconnlistener *listener,
    evutil_socket_t fd,
    struct sockaddr *sa,
    int socklen,
    void *arg) {
    TESTOUT("main thread listen_callback");

    // TODO: create task

    // TODO: dispatch task
}

int main() {
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
        errmsg("signal(SIGPIPE, SIG_IGN)");
    }

    // TODO: init thread pool

    // init event context
    event_base *base = event_base_new();
    if (!base) { errmsg("main thread event_base_new"); }

    sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);

    // listening
    evconnlistener *ev = evconnlistener_new_bind(
        base, // context
        listen_callback,
        base, // arg for listen_callback
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
        10,               // listen backlog
        (sockaddr *)&sin, // bind address
        sizeof(sin));

    if (base) {
        std::cout << "listening on port " << SERVER_PORT << std::endl;
        event_base_dispatch(base);
    }

    if (ev) { evconnlistener_free(ev); }

    if (base) { event_base_free(base); }

    TESTOUT("Sever down.");

    return 0;
}