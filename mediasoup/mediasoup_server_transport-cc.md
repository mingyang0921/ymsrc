# mediasoup server transport-cc

## 一、大概流程
1. twcc包含在tranport的类中,相同的生命周期
2. 接收数据包,更新twcc数据
3. 启动定时器,定时发送rtcp数据包

## 二、调用方法
 - Transport::HandleRequest Channel::ChannelRequest::MethodId::TRANSPORT_PRODUCE 创建
 - Transport析构时销毁
 - Transport::ReceiveRtpPacket时将packet和时间戳传入
 - Transport::Disconnected时调用tccServer->TransportDisconnected
 - Transport::Connected时调用tccServer->TransportConnected
 - Channel::ChannelRequest::MethodId::TRANSPORT_SET_MAX_INCOMING_BITRATE 设置最大接收带宽
 - Transport::FillJsonStats 获取twcc的状态数据
 - 构造时创建了定时器
 - TransportCongestionControlServer::OnTimer 调用TransportCongestionControlServer::SendTransportCcFeedback
 - Transport::OnTransportCongestionControlServerSendRtcpPacket 调用rtcppacket

## 三、协议技术

### 3.1 收发包
 - 客户端发送RTP包到服务器
 - 服务器定时响应TransportFeedback RTCP到客户端