# srs webrtc nack 流程

## 一、逻辑流程
1. srs初始化时创建20ms间隔的定时器
2. 客户端推流时,http请求创建SrsRtcConnection
3. 将SrsRtcConnection加入到20ms间隔的定时器队列中
4. 20ms间隔的定时器通过休眠和循环，进行遍历调用所有定时任务
5. 调用nack定时任务

## 二、源码流程
### 2.1 接收http请求创建SrsRtcConnection
SrsGoApiRtcPublish::serve_http  
SrsRtcServer::create_session  
SrsRtcConnection::SrsRtcConnection  
### 2.2 将timer加入到20ms定时器当中
注释:不创建新的定时器，而是加入到实现准备的20ms定时器的队列中
SrsRtcConnectionNackTimer::SrsRtcConnectionNackTimer  
SrsFastTimer::subscribe  
### 2.3 20ms定时器运行方式
注释：每隔20ms运行一次,可以看出，如果回调较多，
且时间较长，间隔时间会不准确
SrsFastTimer::cycle  
timer->on_timer(interval_))   
srs_usleep(interval_);  
### 2.4 运行nack
SrsRtcConnectionNackTimer::on_timer  
SrsRtcPublishStream::check_send_nacks  
SrsRtcVideoRecvTrack::check_send_nacks or SrsRtcAudioRecvTrack::check_send_nacks  
SrsRtcRecvTrack::do_check_send_nacks  
SrsRtcConnection::check_send_nacks  
SrsRtcConnection::send_rtcp  
## 三、总结
1. nack的rtcp时间间隔大于等于20ms
2. 所以的推流端的nack是统一控制的