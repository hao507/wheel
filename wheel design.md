###wheel design

- 封装C、golang系统调用基础跨平台库
  process、thread、signal、shrmem、utils(read_file, write_file, sysinfo, hardinfo...)



- log模块设计

  支持syslog
  支持Level，可以支持Release and Debug
  可输出调试信息（函数名、文件名、行号。。。）
  可以有多个writer或者pinter（可以支持网络writer或printer）
  可以支持Rotate（按File size or time stamp）
  console支持配色方案。
  可以支持异步执行,使用MQ（多进程：使用共享内存）。
  可支持配置文件，和默认std。
  支持线程锁，进程锁（多进程输出到同一个文件）。



- 文件监控

  跨平台，基于事件模型（CreateEvent、WriteEvent、RenameEvent、RemoveEvent、ChmodEvent、ChownEvent）
  可以设置监控时间（在同一段时间内的所有事件视为一次事件，一次写事件，但某些编辑器行为可能会多次写入文件。）
  某些编辑器会备份之前的文件，修改完毕后，删除原文件，然后rename备份to原文件。（一次写操作，触发两次Event）



- TCP/UDP 打洞

  提供一套库函数API，可以实现server与client 。
  给各自返回对方及自己在公网的标识（IP与port）。



- 配置文件

  基于json配置文件解析(.json)。
  基于Linux配置文件解析（.conf）。
  基于Windows配置文件解析(.ini)。
  基于csv配置文件解析(.csv)。



- 高效网络库（类似libevent）



- 协议层（算法层面）

  UDP实现TCP功能：应用层协议（可以模拟长连接、丢包重补、滑动窗口，【流式 | 报式】）。



- 序列化/反序列化

  二进制序列化 + 压缩



- 轻量级数据库（类似mysql），no-sql（类似redis）




- 分布式存储
  支持集群、高可用



- 分布式计算
Hadoop。。。。。