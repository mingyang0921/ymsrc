# srs server agreetment manager

## 一、代码分析

```cpp
class SrsHybridServer : public ISrsFastTimer
{
private:
    std::vector<ISrsHybridServer*> servers;  // 存储server
    SrsFastTimer* timer20ms_;                // 20ms定时器
    SrsFastTimer* timer100ms_;               // 100ms定时器
    SrsFastTimer* timer1s_;                  // 1s定时器
    SrsFastTimer* timer5s_;                  // 5s定时器
    SrsClockWallMonitor* clock_monitor_;
public:
    SrsHybridServer();
    virtual ~SrsHybridServer();
public:
    virtual void register_server(ISrsHybridServer* svr);  //将server存入动态数组
public:
    virtual srs_error_t initialize();       //启动定时器;初始化所有加入的server
    virtual srs_error_t run();              //运行所有加入动态数组的server
    virtual void stop();
public:
    virtual SrsServerAdapter* srs();        // 返回SrsServer；使用了类格式判断
    SrsFastTimer* timer20ms();              //返回20ms定时器类
    SrsFastTimer* timer100ms();             //返回100ms定时器类
    SrsFastTimer* timer1s();                //返回1s定时器类
    SrsFastTimer* timer5s();                //返回5秒定时器类
// interface ISrsFastTimer
private:
    srs_error_t on_timer(srs_utime_t interval);
};
```
## 二、类作用
如果想新增新协议，只需要模仿下面三种协议  
1. 完成类的初始化、运行、停止函数
2. 实例化服务的类
3. 将新增加的类注册到SrsHybridServer

### 2.1 rtmp协议
srs_app_server.hpp  
```cpp
class SrsServerAdapter : public ISrsHybridServer
{
private:
    SrsServer* srs;
public:
    SrsServerAdapter();
    virtual ~SrsServerAdapter();
public:
    virtual srs_error_t initialize();
    virtual srs_error_t run(SrsWaitGroup* wg);
    virtual void stop();
public:
    virtual SrsServer* instance();
};
```
### 2.2 rtc协议
srs_app_rtc_server.hpp  
```cpp
class RtcServerAdapter : public ISrsHybridServer
{
private:
    SrsRtcServer* rtc;
public:
    RtcServerAdapter();
    virtual ~RtcServerAdapter();
public:
    virtual srs_error_t initialize();
    virtual srs_error_t run(SrsWaitGroup* wg);
    virtual void stop();
};
```

### 2.3 srt协议
srs_app_srt_server.hpp  

```cpp
class SrsSrtServerAdapter : public ISrsHybridServer
{
private:
    SrsSrtServer* srt_server_;
public:
    SrsSrtServerAdapter();
    virtual ~SrsSrtServerAdapter();
public:
    virtual srs_error_t initialize();
    virtual srs_error_t run(SrsWaitGroup* wg);
    virtual void stop();
public:
    virtual SrsSrtServer* instance();
};

```

## 三、新增测试协议
新增简单的rtp/rtcp服务器

### 3.1 新增协议类
srs_app_rtprtcp_server.hpp  
增加协议，初始化，运行，销毁类
```cpp
class RtpRtcpServerAdapter : public ISrsHybridServer
{
private:
    SrsRtpRtcpServer* rtprtcp;
public:
    RtpRtcpServerAdapter();
    virtual ~RtpRtcpServerAdapter();
public:
    virtual srs_error_t initialize();
    virtual srs_error_t run(SrsWaitGroup* wg);
    virtual void stop();
};

```

### 3.2 注册协议
在srs_main_server.cpp中 
1. 引入头文件 #include <srs_app_rtprtcp_server.hpp>
2. 注册类 
在_srs_hybrid->register_server(new RtcServerAdapter());后边加入 
_srs_hybrid->register_server(new RtpRtcpServerAdapter());  