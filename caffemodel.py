# -*- coding:utf-8 -*-
import numpy as np
import caffe
#import collections
#from collections import OrderedDict
caffe.set_mode_cpu
net1 = caffe.Net('deploy.prototxt',\
    'zero_weights.caffemodel',caffe.TEST) #TEST/TRAIN
    #我的python脚本，prototxt,caffemodel文件放在同级目录下了，你的文件路径按需修改
conv1_w = net1.params['conv1/3x3_s1'][0].datal
#模型参数都存在了net.params这个有序字典里，对这就是python里的那个字典，所以对模型参数的操作和对python字典操作一样。['conv1_1/conv']是键名，[0]是权的维度

keys0=net1.params.keys()
#print net0.params.keys()
for key1 in keys0:   # 输出所有层名，参数
    print(key1)
    try:
        print(net1.params[key1][0].data)
    except IndexError:
        continue
    try:
        print(net1.params[key1][1].data)
    except IndexError:
        continue
    try:
        print (net1.params[key1][2].data)
    except IndexError:
        continue
    finally:
        print('\n')
