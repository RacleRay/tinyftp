#pragma once

#include <Task.h>

class TaskFactory {
public:
    static TaskFactory* get() {
        static TaskFactory factory;
        return &factory;
    }

    Task createTask();

private:
    TaskFactory();
};