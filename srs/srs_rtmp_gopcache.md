# srs rtmp gopcache

## 一、作用
gop cache用于播放端快速启动

## 二、代码详解

1. 在SrsLiveSource构造和析构中包含gopcache的创建和释放
2. 在SrsServer类析构中，包含对所有gopcache中的msg的销毁
3. 在SrsLiveSource::on_unpublish 停止推流中也包含对gopcache中的msg的销毁
4. 在SrsLiveSource类中分别封装了设置是否启动gopcache和gopcache的max frames
5.  SrsLiveSource::on_audio_imp和 SrsLiveSource::on_video_imp分别将rtmp msg存入gopcache中
6. 在SrsLiveSource::consumer_dumps中，将gopcache中的msg循环发给客户端

## 三、知识点

### 3.1 一个gopcache的生存周期
在srs_error_t SrsGopCache::cache(SrsSharedPtrMessage* shared_msg)中
```cpp
    // clear gop cache when got key frame
    if (msg->is_video() && SrsFlvVideo::keyframe(msg->payload, msg->size)) {
        clear();
        
        // curent msg is video frame, so we set to 1.
        cached_video_count = 1;
    }
```
当接收到视频帧且是关键帧时，清理掉之前的所有gopcache,所以一个gopcache如下
I-P-AUDIO-P-ADUIO-P-AUIO
两个I帧的所有数据，包含第一个I帧

