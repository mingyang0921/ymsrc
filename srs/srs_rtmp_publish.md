# srs rtmp publish 原理

## 一、实现逻辑
1. 协程A接收客户端连接,完成与客户端交互
2. 启动协程B接收客户端数据和命令
3. 协程A定时打印接收的数据详情和监控客户端超时
4. 协程B接收数据,存入到队列中,并且可以同时消费者


## 二、源码分析

### 2.1 协程A调用栈

```cpp
SrsRtmpConn::cycle  
SrsRtmpConn::do_cycle  
SrsRtmpConn::service_cycle  
SrsRtmpConn::stream_service_cycle  
SrsRtmpServer::start_fmle_publish  
SrsRtmpConn::publishing  
SrsRtmpConn::do_publishing 
```

### 2.2 协程B调用栈

```cpp
SrsRecvThread::cycle
SrsRecvThread::do_cycle
SrsPublishRecvThread::consume
SrsRtmpConn::handle_publish_message
SrsRtmpConn::process_publish_message

// 视频分支
SrsLiveSource::on_video
SrsLiveSource::on_video_imp
SrsLiveConsumer::enqueue       // for循环遍历所有消费者
SrsMessageQueue::enqueue       // 将msg插入到msgs队列中
SrsMessageQueue::shrink        // 缓存数据超过配置进行销毁

// 音频分支
SrsLiveSource::on_audio
SrsLiveSource::on_audio_imp
SrsLiveConsumer::enqueue       // for循环遍历所有消费者
SrsMessageQueue::enqueue       // 将msg插入到msgs队列中
SrsMessageQueue::shrink        // 缓存数据超过配置进行销毁
```
