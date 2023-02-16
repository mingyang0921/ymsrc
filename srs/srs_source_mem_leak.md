# srs source memory leak


## 一、问题起因
由于在source释放时,关联consume/publish等资源,在释放时容易出现异常问题。
所以srs开发者将释放source的代码进行了注释（代码如下）。所以会造成内存持续升高。
```cpp
srs_error_t SrsLiveSourceManager::notify(int event, srs_utime_t interval, srs_utime_t tick)
{
    srs_error_t err = srs_success;

    std::map<std::string, SrsLiveSource*>::iterator it;
    for (it = pool.begin(); it != pool.end();) {
        SrsLiveSource* source = it->second;

        // Do cycle source to cleanup components, such as hls dispose.
        if ((err = source->cycle()) != srs_success) {
            return srs_error_wrap(err, "source=%s/%s cycle", source->source_id().c_str(), source->pre_source_id().c_str());
        }

        // TODO: FIXME: support source cleanup.
        // @see https://github.com/ossrs/srs/issues/713
        // @see https://github.com/ossrs/srs/issues/714
#if 0
        // When source expired, remove it.
        if (source->expired()) {
            int cid = source->source_id();
            if (cid == -1 && source->pre_source_id() > 0) {
                cid = source->pre_source_id();
            }
            if (cid > 0) {
                _srs_context->set_id(cid);
            }
            srs_trace("cleanup die source, total=%d", (int)pool.size());

            srs_freep(source);
            pool.erase(it++);
        } else {
            ++it;
        }
#else
        ++it;
#endif
    }

    return err;
}
```

## 二、解决办法

### 2.1 延长释放时间
1. 打开上面注释掉的代码
2. SRS_SOURCE_CLEANUP 由30秒，改为1小时

### 2.2 重启服务器
1. srs提供了热启动等功能。