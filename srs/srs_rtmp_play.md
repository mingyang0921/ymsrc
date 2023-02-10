# srs rtmp play 原理

## 一、实现逻辑

1. 协程A接收客户端连接,完成与客户端交互
2. 启动协程B接收客户端命令
3. 协程A打印接收数据,超时或等待事件,进行获取数据，发送数据给客户端


## 二、源码分析

### 2.1 协程A调用栈

```cpp
SrsRtmpConn::cycle
SrsRtmpConn::do_cycle
SrsRtmpConn::service_cycle
SrsRtmpConn::stream_service_cycle
SrsRtmpConn::playing
SrsRtmpConn::do_playing
SrsRtmpServer::send_and_free_messages  //发送数据
```

### 2.2 协程B调用栈

```cpp
SrsRecvThread::cycle
SrsRecvThread::do_cycle
SrsPublishRecvThread::consume
SrsRtmpConn::handle_publish_message
SrsRtmpConn::process_publish_message


```





