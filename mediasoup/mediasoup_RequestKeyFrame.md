# mediasoup 获取关键帧

## 一、为什么要有此功能
因为rtc为了减少流量,关键帧间隔时间较大,可能造成后加入的客户端起播时间太长。

## 二、流程概述
1. 播放端使用websocket向js server发送获取关键帧请求
2. js server收到请求后像mediasoup-worker发送unix-socket请求
3. mediasoup-worker解析请求
4. mediasoup-worker通过comsumer查找到producer
5. 通知producer请求关键帧
6. 发送rtcp数据包到客户端请求关键帧

## 三、具体流程
1. Consumer.js requestKeyFrame()
2. Channel.js request(method, internal, data)
3. Channel::ChannelRequest::MethodId::CONSUMER_REQUEST_KEY_FRAME //收到业务回调
 - SvcConsumer::HandleRequest
 - SimulcastConsumer::HandleRequest
 - SimpleConsumer::HandleRequest
 - PipeConsumer::HandleRequest
4. RequestKeyFrame  //收到请求关键帧请求
 - SvcConsumer::RequestKeyFrame
 - SimulcastConsumer::RequestKeyFrames
 - SimpleConsumer::RequestKeyFrame
 - PipeConsumer::RequestKeyFrame
5. Transport::OnConsumerKeyFrameRequested
6. Router::OnTransportConsumerKeyFrameRequested
7. RequestKeyFrame
 - SvcConsumer::RequestKeyFrame
 - SimpleConsumer::RequestKeyFrame
 - PipeConsumer::RequestKeyFrame
8. Transport::OnConsumerKeyFrameRequested
9. Router::OnTransportConsumerKeyFrameRequested
10. RtpStreamRecv::RequestKeyFrame
11. Producer::OnRtpStreamSendRtcpPacket
12. Producer::EmitTraceEventPliType
13. ChannelNotifier::Emit
14. ChannelSocket::Send
15. ChannelSocket::SendImpl
16. UnixStreamSocket::Write //unix-socket发送
17. uv_write