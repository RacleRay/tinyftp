# ftpd

- 分命令通道和数据通道，建立不同的连接
- CmdServer类，负责命令通道的管理，有自己的 event_base
- 不同的 Task 对应不同的类型：FtpUSER, FtpRETR 等等，每个类注册了不同的事件回调函数
- Task 的 event_base 对于只在命令通道工作的FtpUSER这种命令而言，与 CmdServer 的 event_base 相同；使用数据通道时，event_base 需要另建
- Thread 的 event_base 只用于线程内部异步通知，不涉及 Task 的执行过程
- Task 建立后保存在 Thread 的 task_list 中，由一个互斥锁保护
- ThreadPool 简单的按顺序循环分配工作线程
- 工厂+单例，处理每个连接的命令通道管理类 CmdServer 的对象


> 已经做了一些改动，但是这套代码的改进空间还很大。用于学习 ftp 和 libevent 还是有用的。