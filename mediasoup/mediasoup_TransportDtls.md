# mediasoup transport dtls


## 一、总叙述
1. webrtc使用https/wss进行createWebRtcTransport或connect
2. createWebRtcTransport返回dtlsParameters给客户端
3. 客户端connect时,使用dtlsParameters,并使用ssl握手
4. 客户端使用tcp/udp连接

## 二、TCP调用流程

### 2.1 tcp的建立

#### 2.1.1 tcp是被谁建立的
 - router收到创建webrtctransport的请求
 - 创建webrtctransport类
 - 解析router传入大参数，获取到enableTcp选项
 - 创建TcpServer类

#### 2.1.2 tcp创建和调用流程
 - Router::HandleRequest             //router收到创建webrttransport的请求
 - WebRtcTransport::WebRtcTransport  //产生WebRtcTransport
 - TcpServer::TcpServer              //解析参数,使用tcp协议,创建TcpServer
 - TcpServerHandler::TcpServerHandler //一下为通过libuv设置回调等
 - onConnection
 - TcpServerHandler::OnUvConnection
 - TcpServer::UserOnTcpConnectionAlloc
 - TcpServerHandler::AcceptTcpConnection
 - TcpConnectionHandler::Start
 - onRead
 - TcpConnectionHandler::OnUvRead
 - TcpConnection::UserOnTcpConnectionRead
 - WebRtcTransport::OnTcpConnectionPacketReceived //webrtc收到tcp回调

### 2.2 处理流程

 - WebRtcTransport::OnTcpConnectionPacketReceived
 - WebRtcTransport::OnPacketReceived
 - WebRtcTransport::OnDtlsDataReceived
 - DtlsTransport::ProcessDtlsData
 - DtlsTransport::CheckStatus
 - - DtlsTransport::ProcessHandshake
 - - DtlsTransport::CheckRemoteFingerprint //指纹判断
 - - DtlsTransport::ExtractSrtpKeys //生成srtp加密和解密
 - WebRtcTransport::OnDtlsTransportApplicationDataReceived
 - - Transport::ReceiveSctpData
 - - SctpAssociation::ProcessSctpData  //返回处理成功


## 三、UDP处理流程

### 3.1 udp的建立

#### 3.1.1 udp是被谁建立的
 - router收到创建webrtctransport的请求
 - 创建webrtctransport类
 - 解析router传入大参数，获取到enableUdp选项
 - 创建UdpSocket类

#### 3.1.2 udp创建和调用流程
 - Router::HandleRequest             //router收到创建webrttransport的请求
 - WebRtcTransport::WebRtcTransport  //产生WebRtcTransport
 - UdpSocket::UdpSocket
 - UdpSocketHandler::UdpSocketHandler
 - onRecv
 - UdpSocketHandler::OnUvRecv
 - UdpSocket::UserOnUdpDatagramReceived
 - OnUdpSocketPacketReceived  // 每种协议都创建了udp
 - PipeTransport::OnUdpSocketPacketReceived
 - PlainTransport::OnUdpSocketPacketReceived
 - WebRtcTransport::OnUdpSocketPacketReceived
 - WebRtcTransport::OnPacketReceived

### 3.2 处理流程
 - WebRtcTransport::OnDtlsDataReceived
 - 同2.2

## 四、srtp的key交换

 - DtlsTransport::ProcessHandshake
 - DtlsTransport::ExtractSrtpKeys
 - WebRtcTransport::OnDtlsTransportConnected
 - 