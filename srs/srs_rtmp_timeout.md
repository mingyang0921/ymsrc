# 一、背景
  在调试srs代码时，发现log中的超时时间有时候不一样，且自己也没有配置过相关参数，所以决定仔细看看源码，分析下原因。
    
# 二、推流超时规则
1. 连接握手结束以后，如果还未接收到真实的音视频包，超时时间默认为20秒
2. 连接握手结束之后，如果接收过真实的音视频包，超时时间为5秒。

# 三、推流超时代码分析
  代码一共两个协程，一个是握手协程，然后定时循环，
  一个是握手结束后，创建的新协程，专门用于接收数据。代码主要是
  SrsRtmpConn::do_publishing和SrsPublishRecvThread::consume。
  
1. 获取超时时间代码
```cpp
publish_1stpkt_timeout = _srs_config->get_publish_1stpkt_timeout(req->vhost);  
publish_normal_timeout = _srs_config->get_publish_normal_timeout(req->vhost); 
```
3. 判断是否接收过数据包代码
```cpp
 if (nb_msgs == 0) {        // 根据是否接受过数据包，进行不同的超时时间等待
        // when not got msgs, wait for a larger timeout.
        // @see https://github.com/ossrs/srs/issues/441
        rtrd->wait(publish_1stpkt_timeout);
    } else {
       rtrd->wait(publish_normal_timeout);
    }
```
4. 超时退出代码
```cpp
    if (rtrd->nb_msgs() <= nb_msgs) {    // 如果超时后，接收的数据包没有增长，则退出
        return srs_error_new(ERROR_SOCKET_TIMEOUT, "rtmp: publish timeout %dms, nb_msgs=%d",
            nb_msgs? srsu2msi(publish_normal_timeout) : srsu2msi(publish_1stpkt_timeout), (int)nb_msgs);
    }
    nb_msgs = rtrd->nb_msgs();
```
5. 接收的数据包的数量计算位置
```cpp
srs_error_t SrsPublishRecvThread::consume(SrsCommonMessage* msg)
{
    srs_error_t err = srs_success;
    
    // when cid changed, change it.
    if (ncid.compare(cid)) {
        _srs_context->set_id(ncid);
        cid = ncid;
    }
    
    _nb_msgs++;    // 接收到一个数据包，进行++
    
    if (msg->header.is_video()) {
        video_frames++;
    }
    
    // log to show the time of recv thread.
    srs_verbose("recv thread now=%" PRId64 "us, got msg time=%" PRId64 "ms, size=%d",
                srs_update_system_time(), msg->header.timestamp, msg->size);
    
    // the rtmp connection will handle this message
    err = _conn->handle_publish_message(_source, msg);
    
    // must always free it,
    // the source will copy it if need to use.
    srs_freep(msg);
    
    if (err != srs_success) {
        return srs_error_wrap(err, "handle publish message");
    }

    // Yield to another coroutines.
    // @see https://github.com/ossrs/srs/issues/2194#issuecomment-777463768
    if (++nn_msgs_for_yield_ >= 15) {
        nn_msgs_for_yield_ = 0;
        srs_thread_yield();
    }
    
    return err;
}
```
# 四、超时问题扩展
  之前以为librtmp有参数设置超时时间，以为可以使用客户端设置，最后查看librtmp代码，发现代码
```cpp
SET_RCVTIMEO(tv, r->Link.timeout);
if (setsockopt(r->m_sb.sb_socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv)))
```
设置的是socket的超时时间，跟rtmp协议无关。