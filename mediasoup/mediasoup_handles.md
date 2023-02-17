# mediasoup handles

## 一、目录文件
1. SignalsHandler.cpp
2. TcpConnectionHandler.cpp
3. TcpServerHandler.cpp
4. Timer.cpp
5. UdpSocketHandler.cpp
6. UnixStreamSocket.cpp


## 二、具体功能分析

### 2.1 SignalsHandler.cpp
1. worker注册SIGINT,SIGTERM信号
```cpp
this->signalsHandler->AddSignal(SIGINT, "INT");
this->signalsHandler->AddSignal(SIGTERM, "TERM");
```
2. 继承类后,重写OnSignal虚函数,接收到信号时，释放worker资源
```cpp
inline void Worker::OnSignal(SignalsHandler* /*signalsHandler*/, int signum)
{
	MS_TRACE();

	if (this->closed)
		return;

	switch (signum)
	{
		case SIGINT:
		{
			if (this->closed)
				return;

			MS_DEBUG_DEV("INT signal received, closing myself");

			Close();

			break;
		}

		case SIGTERM:
		{
			if (this->closed)
				return;

			MS_DEBUG_DEV("TERM signal received, closing myself");

			Close();

			break;
		}

		default:
		{
			MS_WARN_DEV("received a non handled signal [signum:%d]", signum);
		}
	}
}
```
### 2.2 UnixStreamSocket.cpp

1. UnixStreamSocket构造函数中
1）fd绑定
2）role为消费者时,启动read
3）UnixStreamSocket::OnUvRead中调用UserOnUnixStreamRead
4）UserOnUnixStreamRead为虚函数，需要子类继承实现
5）UserOnUnixStreamRead被ChannelSocket中的ConsumerSocket实现
6）UserOnUnixStreamRead被PayloadChannelSocket中的ConsumerSocket实现

2. UnixStreamSocket因为是stream,所以只有只有comsumer才有uvread

以上基本实现了跨worker之间消费数据

### 2.3 UdpSocketHandler.cpp

1）写法同2.2 基本就是实现read回调和send发送函数
2）回调中调用UserOnUdpDatagramReceived
3）UserOnUdpDatagramReceived由UdpSocket类实现
4）并调用OnUdpSocketPacketReceived虚函数
5）分别被Pipetransport,webrtctransport,webrtcserver实现虚函数
6）然后最后都基本调用 rtp的处理函数

以上就完成udp数据流的入口

### 2.4 Timer.cpp

```cpp
Transport.cpp (e:\svn_client\github.com\mediasoup\worker\src\rtc):		this->rtcpTimer = new Timer(this);
Transport.cpp (e:\svn_client\github.com\mediasoup\worker\src\rtc):		delete this->rtcpTimer;
Transport.cpp (e:\svn_client\github.com\mediasoup\worker\src\rtc):		this->rtcpTimer = nullptr;
Transport.cpp (e:\svn_client\github.com\mediasoup\worker\src\rtc):		this->rtcpTimer->Start(static_cast<uint64_t>(RTC::RTCP::MaxVideoIntervalMs / 2));
Transport.cpp (e:\svn_client\github.com\mediasoup\worker\src\rtc):		this->rtcpTimer->Stop();
Transport.cpp (e:\svn_client\github.com\mediasoup\worker\src\rtc):		if (timer == this->rtcpTimer)
Transport.cpp (e:\svn_client\github.com\mediasoup\worker\src\rtc):			this->rtcpTimer->Start(interval);
Transport.hpp (e:\svn_client\github.com\mediasoup\worker\include\rtc):		Timer* rtcpTimer{ nullptr };

```
通过查看timer的调用方法例子，基本可以得知调用方法。   
Transport::OnTimer重新实现，重写虚函数完成回调的就提方法。  

### 2.5 TcpServerHandler.cpp
基本实现对libuv tcp服务器的封装。
TcpServer继承此类。实现libuv的调用隔离


### 2.6 TcpConnectionHandler.cpp
...
