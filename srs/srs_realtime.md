# srs realtime原理

## 一、实现逻辑

1. 优化tcp socket 网络
2. 优化客户端播放时，服务器向客户端发送数据包的最小间隔时间和发包最低包数量
3. 关闭缓存gop,降低延迟,同时增加了开屏事件。
4. 设置
5. 推流是关闭mr

## 二、源码分析

### 2.1 配置文件

#### 2.1.1 配置文件内容
```
listen              1935;
max_connections     1000;
daemon              off;
srs_log_tank        console;
vhost __defaultVhost__ {
    tcp_nodelay     on
    min_latency     on;
    
    play {
        gop_cache       off;
        queue_length    10;
        mw_latency      100;
    }
    
    publish {
        mr off;
    }
}
```
#### 2.1.2 配置文件重要分析
1. ```tcp_nodelay     on;```
TCP_NODELAY选项禁止Nagle算法。Nagle算法通过将未确认的数据存入缓冲区直到蓄足一个包一起发送的方法，来减少主机发送的零碎小数据包的数目。但对于某些应用来说，这种算法将降低系统性能。所以TCP_NODELAY可用来将此算法关闭。应用程序编写者只有在确切了解它的效果并确实需要的情况下，才设置TCP_NODELAY选项，因为设置后对网络性能有明显的负面影响。TCP_NODELAY是唯一使用IPPROTO_TCP层的选项，其他所有选项都使用SOL_SOCKET层。

2. ```min_latency     on;```
用于设置是否开启最小延迟,依赖下文中play的mw_latency  
3.  ```   
    play {
        gop_cache       off;
        queue_length    10;
        mw_latency      100;
    }
    ```
用于设置play时的参数  
4. ```
    publish {
        mr off;
    }
    ```
用于设置publish时的参数  
### 2.2 源码分析
#### 2.2.1 优化tcp_nodelay
1. SrsRtmpConn::do_publishing or SrsRtmpConn::playing 即播放和推流时都调用了SrsRtmpConn::set_sock_options  
2. SrsConfig::get_tcp_nodelay 获取是否开启tcp_nodelay
3. SrsTcpConnection::set_tcp_nodelay 设置tcp_nodelay
4. 使用getsockopt or setsockopt 查看和设置tcp_nodelay
5. 优化意义 同2.1.2 1中的讲解

#### 2.2.2 优化 min_latency
1. SrsConfig::get_realtime_enabled 用来判断是否启用min_latency，其中rtc是默认启用，rtmp是默认不启用  
2. SrsConfig::get_mw_msgs 用来获取可以发包的最小值，其中如何设置了mw_msgs，按照实际设置的值(本配置文件未设置),没有设置，rtc默认为1，rtmp默认为0(也就是依赖最小间隔时间,跟待发包数量无关了)  
3. SrsConfig::get_mw_sleep 用来获取最小发包时间间隔,本配置文件设置的是mw_latency是100，也就是100毫秒。  
4. SrsLiveConsumer::wait 中,分别通过SrsConfig::get_mw_msgs和SrsConfig::get_mw_sleep获取额参数,使用srs_cond_wait进行等待事件触发。

#### 2.2.3 优化play中的gop_cache

#### 2.2.4 优化play中的queue_length
1. SrsConfig::get_queue_length 获取设置参数  
2. SrsLiveSource::consumer_dumps中的SrsLiveConsumer::set_queue_size 设置消费者的队列长度  
3. SrsMessageQueue::enqueue 中通过获取队列中的第一个包和最后一个包的时间，如果小于queue_length，则进行

#### 2.2.5 优化publish中的mr

## 三、总结
