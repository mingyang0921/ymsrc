# mediasoup

## 一、类介绍
### 1.1 继承关系
父类:
1. UnixStreamSocket
子类:
1. PayloadChannel::ConsumerSocket
2. Channel::ConsumerSocket

### 1.2 数据回调
 - UnixStreamSocket::OnUvRead
 - UserOnUnixStreamRead

#### 1.2.1 PayloadChannel回调
 - PayloadChannelSocket::OnConsumerSocketMessage

#### 1.2.2 Channel回调

#### 1.2.2.1 worker回调
 - ChannelSocket::OnConsumerSocketMessage
 - new Channel::ChannelRequest
1. this->channel->SetListener(this);
2. this->payloadChannel->SetListener(this);
 - Worker::OnChannelRequest // worker接口
1. Channel::ChannelRequest::MethodId::WORKER_CLOSE: //关闭worker
2. Channel::ChannelRequest::MethodId::WORKER_DUMP //获取router信息列表
3. Channel::ChannelRequest::MethodId::WORKER_GET_RESOURCE_USAGE: //获取当前进程资源
4. Channel::ChannelRequest::MethodId::WORKER_UPDATE_SETTINGS: //更新配置文件
5. Channel::ChannelRequest::MethodId::WORKER_CREATE_ROUTER: //创建router
6. Channel::ChannelRequest::MethodId::ROUTER_CLOSE:  //删除router
7. default Router::HandleRequest(Channel::ChannelRequest* request)

#### 1.2.2.2 router回调
 -  Router::HandleRequest(Channel::ChannelRequest* request)
1. Channel::ChannelRequest::MethodId::ROUTER_DUMP:
2. Channel::ChannelRequest::MethodId::ROUTER_CREATE_WEBRTC_TRANSPORT:
3. Channel::ChannelRequest::MethodId::ROUTER_CREATE_PLAIN_TRANSPORT:
4. Channel::ChannelRequest::MethodId::ROUTER_CREATE_PIPE_TRANSPORT:
5. Channel::ChannelRequest::MethodId::ROUTER_CREATE_DIRECT_TRANSPORT:
6. Channel::ChannelRequest::MethodId::ROUTER_CREATE_ACTIVE_SPEAKER_OBSERVER:
7. Channel::ChannelRequest::MethodId::ROUTER_CREATE_AUDIO_LEVEL_OBSERVER:
8. Channel::ChannelRequest::MethodId::TRANSPORT_CLOSE:
9. Channel::ChannelRequest::MethodId::RTP_OBSERVER_CLOSE:
10. Channel::ChannelRequest::MethodId::RTP_OBSERVER_PAUSE:
11. Channel::ChannelRequest::MethodId::RTP_OBSERVER_RESUME:
12. Channel::ChannelRequest::MethodId::RTP_OBSERVER_ADD_PRODUCER:
13. Channel::ChannelRequest::MethodId::RTP_OBSERVER_REMOVE_PRODUCER:
14. default Transport::HandleRequest

#### 1.2.2.3 transport回调
 - Transport::HandleRequest
1. Channel::ChannelRequest::MethodId::TRANSPORT_DUMP:
2. Channel::ChannelRequest::MethodId::TRANSPORT_GET_STATS:
3. Channel::ChannelRequest::MethodId::TRANSPORT_SET_MAX_INCOMING_BITRATE:
4. Channel::ChannelRequest::MethodId::TRANSPORT_SET_MAX_OUTGOING_BITRATE:
5. Channel::ChannelRequest::MethodId::TRANSPORT_PRODUCE:
6. Channel::ChannelRequest::MethodId::TRANSPORT_CONSUME:
7. Channel::ChannelRequest::MethodId::TRANSPORT_PRODUCE_DATA:
8. Channel::ChannelRequest::MethodId::TRANSPORT_CONSUME_DATA:
9. Channel::ChannelRequest::MethodId::TRANSPORT_ENABLE_TRACE_EVENT:
10. Channel::ChannelRequest::MethodId::PRODUCER_CLOSE:
11. Channel::ChannelRequest::MethodId::CONSUMER_CLOSE:
12. producer 
13. comsumer
14. Channel::ChannelRequest::MethodId::DATA_PRODUCER_CLOSE:
15. Channel::ChannelRequest::MethodId::DATA_CONSUMER_CLOSE:
16. data producer
17. data comsumer
18. Channel::ChannelRequest::MethodId::DATA_CONSUMER_GET_BUFFERED_AMOUNT:
19. Channel::ChannelRequest::MethodId::DATA_CONSUMER_SET_BUFFERED_AMOUNT_LOW_THRESHOLD:

    因为在创建transport时候,已经分成webrtc/plain/pipe所以默认的调用的是webrtctransport
    然后在webrtctransport调用父类transport的回调。
##### 1.2.2.3.1 webrtc
 - WebRtcTransport::HandleRequest
1. Channel::ChannelRequest::MethodId::TRANSPORT_CONNECT
2. Channel::ChannelRequest::MethodId::TRANSPORT_RESTART_ICE
##### 1.2.2.3.2 plain
 - PlainTransport::HandleRequest
1. Channel::ChannelRequest::MethodId::TRANSPORT_CONNECT

##### 1.2.2.3.3 pipe
 - PipeTransport::HandleRequest
1. Channel::ChannelRequest::MethodId::TRANSPORT_CONNECT

#### 1.2.2.4 producer回调
- Producer::HandleRequest(Channel::ChannelRequest* request)
1. Channel::ChannelRequest::MethodId::PRODUCER_DUMP:
2. Channel::ChannelRequest::MethodId::PRODUCER_GET_STATS:
3. Channel::ChannelRequest::MethodId::PRODUCER_PAUSE:
4. Channel::ChannelRequest::MethodId::PRODUCER_RESUME:
5. Channel::ChannelRequest::MethodId::PRODUCER_ENABLE_TRACE_EVENT:
#### 1.2.2.5 comsumer回调
 - Consumer::HandleRequest(Channel::ChannelRequest* request)
1. Channel::ChannelRequest::MethodId::CONSUMER_DUMP:
2. Channel::ChannelRequest::MethodId::CONSUMER_GET_STATS:
3. Channel::ChannelRequest::MethodId::CONSUMER_PAUSE:
4. Channel::ChannelRequest::MethodId::CONSUMER_RESUME:
5. Channel::ChannelRequest::MethodId::CONSUMER_SET_PREFERRED_LAYERS:
6. Channel::ChannelRequest::MethodId::CONSUMER_SET_PRIORITY:
7. Channel::ChannelRequest::MethodId::CONSUMER_REQUEST_KEY_FRAME:
8. case Channel::ChannelRequest::MethodId::CONSUMER_ENABLE_TRACE_EVENT:

    因为在创建comsumer的时候，是按照PipeConsumer/SimpleConsumer/SimulcastConsumer/SvcConsumer
    格式创建的，所以先执行子类的回到，然后通过子类调用父类的

##### 1.2.2.5.1 pipecomsumer
 - PipeConsumer::HandleRequest
1. Channel::ChannelRequest::MethodId::CONSUMER_REQUEST_KEY_FRAME
2. Channel::ChannelRequest::MethodId::CONSUMER_SET_PREFERRED_LAYERS

##### 1.2.2.5.2 
 - SimpleConsumer::HandleRequest
1. Channel::ChannelRequest::MethodId::CONSUMER_REQUEST_KEY_FRAME
2. Channel::ChannelRequest::MethodId::CONSUMER_SET_PREFERRED_LAYERS

#### 1.2.2.6 data producer回调
DataProducer::HandleRequest
1. Channel::ChannelRequest::MethodId::DATA_PRODUCER_DUMP:
2. Channel::ChannelRequest::MethodId::DATA_PRODUCER_GET_STATS:
#### 1.2.2.7 comsumer回调
 - DataConsumer::HandleRequest(Channel::ChannelRequest* request)
1. Channel::ChannelRequest::MethodId::DATA_CONSUMER_DUMP:
2. Channel::ChannelRequest::MethodId::DATA_CONSUMER_GET_STATS:

