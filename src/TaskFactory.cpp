#include <DebugTool.h>
#include <ServerCMD.h>
#include <TaskFactory.h>
#include <FtpTask.h>


Task TaskFactory::createTask() {
    TESTOUT("TaskFactory::createTask(): creating SERVER supported tasks.\n");

    CmdServer cmd = CmdServer();

    // regist cmd
    
}