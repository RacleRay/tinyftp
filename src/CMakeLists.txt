
add_executable(server main.cpp Thread.cpp ThreadPool.cpp TaskFactory.cpp 
    FtpLIST.cpp FtpPORT.cpp FtpRETR.cpp FtpSTOR.cpp FtpTask.cpp FtpUSER.cpp 
    ServerCMD.cpp)

target_link_options(server PRIVATE -levent -lpthread)