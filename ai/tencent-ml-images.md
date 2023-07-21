# tencent-ml-images

## 一、安装部署开发环境

### 1.1 环境部署
见其他

### 1.2 创建创建


```
conda create -n name python=3.6
```


## 二、进入环境


1. 查询所有的环境
```
(base) C:\Users\ym>conda env list
# conda environments:
#
base                  *  E:\anaconda
TF1.6                    E:\anaconda\envs\TF1.6
TF2.9                    E:\anaconda\envs\TF2.9
```

2. 进入某个环境
```
activate  TF2.9 #进入环境
deactivate # 退出环境

jupyter notebook
```

## 三、执行命令

```
python image_classification.py --images=data/im_list_for_classification.txt  --top_k_pred=5  --model_dir=checkpoints/resnet.ckpt  --dictionary=data/imagenet2012_dictionary.txt
```

## 四、报错解决

### 4.1 删掉多余信息

#### 4.1.1 原因
 - 多余信息
```
(TF2.9) E:\svn_client\github.com\tencent-ml-images>python image_classification.py --images=data/im_list_for_classification.txt  --top_k_pred=5  --model_dir=checkpoints/resnet.ckpt  --dictionary=data/imagenet2012_dictionary.txt
  File "E:\svn_client\github.com\tencent-ml-images\image_classification.py", line 12
    from __future__ import absolute_import
    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
SyntaxError: from __future__ imports must occur at the beginning of the file
```

#### 4.1.2 解决办法
 - 删掉以下
```
"""
Tencent is pleased to support the open source community by making Tencent ML-Images available.
Copyright (C) 2018 THL A29 Limited, a Tencent company. All rights reserved.
Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
https://opensource.org/licenses/BSD-3-Clause
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.
"""

"""Use the saved checkpoint to run single-label image classification"""
```


### 4.2 版本过旧

#### 4.2.1 原因

 - 安装的tensorflow是2.0版本的，所以会报这个错误。

```
Traceback (most recent call last):
  File "E:\svn_client\github.com\tencent-ml-images\image_classification.py", line 11, in <module>
    from models import resnet as resnet
  File "E:\svn_client\github.com\tencent-ml-images\models\resnet.py", line 14, in <module>
    from flags import FLAGS
  File "E:\svn_client\github.com\tencent-ml-images\flags.py", line 11, in <module>
    FLAGS = tf.app.flags.FLAGS
AttributeError: module 'tensorflow' has no attribute 'app'
```

#### 4.2.2 解决办法

```
import tensorflow.compat.v1 as tf
FLAGS = tf.app.flags.FLAGS

```