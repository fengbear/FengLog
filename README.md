# FengLog
抄个日志系统, 借鉴haha_log 与 muduo 与 sylar 

### 0、伪类图

![img](./photos/log.png)

### 1、单线程打印100万条日志

+ 同步日志耗时
![img](./photos/sync_single.jpg)

### 2、4线程线程池打印100万条日志

+ 同步日志耗时
![img](./photos/sync.jpg)

+ 异步日志耗时
![img](./photos/async.jpg)