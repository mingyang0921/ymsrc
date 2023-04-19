# mediasoup transport dtls


## 一、总叙述
### 1.1 webrtc连接过程
1. 通过服务器使用安全协议(https/wss等)获取对端的sdp
2. 双方dtls协商连接后,通过SSL_get_peer_certificate获取对端fingerprint
3. 比较sdp中的fingerprint,相同表示连接成功
4. 调动SSL_export_keying_material接口获取srtp接收和发送秘钥对
5. 初始化接收和发送srtp
6. 接收rtp/rtcp消息，使用recv srtp进行解码
7. 发送rtp/rtcp消息，使用send srtp进行编码

### 1.2 dtls的作用
dtls主要作用是交换srtp的秘钥对

### 1.3 技术
1. https/wss
2. ssl/dtls
3. srtp

### 1.4 mediasoup如何调用
1. mediasoup接收wss消息通过unixsocket传递给worker
2. woker找到router创建webrtcTransport
3. 建立tcp/udp server，等待客户端进行dtls连接
4. dtls连接成功后，交换srtp密钥对
5. 发送加密数据

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