#pragma once

#include <ServerCMD.h>

class TaskFactory {
public:
    static TaskFactory* get() {
        static TaskFactory factory;
        return &factory;
    }

    CmdServer createTask();

private:
    TaskFactory() {};
};