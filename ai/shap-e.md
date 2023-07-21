#shap-e


```
cd shape-e
pip install -e .
pip install blobfile
pip install pyyaml
pip install ipywidgets
```


## 一、windows下安装

```

介绍不能使用gpu的问题
https://www.cnblogs.com/YeungBob/p/14490293.html

介绍下载cuda和cudnn
https://blog.csdn.net/XunCiy/article/details/89070315

cuda地址
https://developer.nvidia.com/cuda-toolkit-archive

cudnn地址

https://developer.nvidia.com/rdp/cudnn-archive


各种工具之间的关系
https://blog.csdn.net/qq_42406643/article/details/109545766


anaconda下载
https://www.anaconda.com/download#Downloads


pytorch配置各种版本
https://pytorch.org/get-started/locally/


安装cuda kit和cudnn
https://zhuanlan.zhihu.com/p/94220564

```


```
 - 判断是否安装了cuda
 - import torch print(torch.cuda.is_available())
 - 返回True，cuda就安装好了。

 - 判断是否安装了cuDNN
 - from torch.backends import  cudnn print(cudnn.is_available())
 - 返回True，cudnn就安装好了。

 - 测试代码

import torch
import time

a = torch.randn(400,600,600).to("cuda:0")

b = torch.randn(400,600,600).to("cuda:0")

start_time = time.time()

for i in range(1,1000):
    c = a*b

end_time = time.time()

print("CUDA time: ",(end_time-start_time))


a = torch.randn(400,600,600)

b = torch.randn(400,600,600)

start_time = time.time()

for i in range(1,1000):
    c = a*b

end_time = time.time()

print("CPU time: ",(end_time-start_time))****


```