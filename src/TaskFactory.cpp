#include <DebugTool.h>
#include <ServerCMD.h>
#include <TaskFactory.h>
#include <FtpLIST.h>
#include <FtpTask.h>
#include <FtpUSER.h>
#include <FtpPORT.h>
#include <FtpRETR.h>
#include <FtpSTOR.h>


CmdServer TaskFactory::createTask() {
    TESTOUT("TaskFactory::createTask(): creating SERVER supported tasks.\n");

    CmdServer cmd = CmdServer();

    // regist cmd
    cmd.regist("USER", new FtpUSER());
    cmd.regist("PORT", new FtpPORT());
    cmd.regist("RETR", new FtpRETR());
    cmd.regist("STOR", new FtpSTOR());
    
    FtpTask* list = new FtpLIST();
    cmd.regist("LIST", list);
    cmd.regist("PWD", list);
    cmd.regist("CWD", list);
    cmd.regist("CDUP", list);

    return cmd;
}